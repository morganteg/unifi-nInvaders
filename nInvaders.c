/**
* nInvaders - a space invaders clone for ncurses
* Copyright (C) 2002-2003 Dettus
  *
  * This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* aint64_t with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
*
* homepage: ninvaders.sourceforge.net
* mailto: ninvaders-devel@lists.sourceforge.net
*
*/

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdint.h>
#include "nInvaders.h"
#include "player.h"
#include "aliens.h"
#include "ufo.h"

#define FPS 50

static int32_t lives;
static int64_t score;
static int32_t status; // status handled in timer

#define GAME_LOOP 1
#define GAME_NEXTLEVEL 2
#define GAME_PAUSED 3
#define GAME_OVER 4
#define GAME_EXIT 5
#define GAME_HIGHSCORE 6


static void drawscore(void)
{
  statusDisplay(level, score, lives);
}

/**
 * initialize level: reset attributes of most units
 */
static void initLevel(void)
{
  playerReset();
  aliensReset();
  ufoReset();
  bunkersReset();
  drawscore();
}


/**
 * evaluate command line parameters 
 */
static void evaluateCommandLine(int32_t argc, int8_t **argv)
{
  int32_t argcMod = argc;
  
  // -l : set skill level
  if (argcMod == 3 && strcmp(argv[1], "-l") == 0) {
    if (argv[2][0] >= '0' && argv[2][0] <= '9') {
      skill_level = argv[2][0] - 48;
    } else {
      argcMod = 2;
    }
  }

  // -gpl : show GNU GPL
  if (argcMod == 2 && strcmp(argv[1], "-gpl") == 0) {
    showGpl();
  }

  // wrong command line: show usage
  if (argcMod == 2 || (argcMod == 3 && strcmp(argv[1], "-l") != 0)) {
    showVersion();
    showUsage();
    exit(1);
  }
}


static void finish(void)
{
  (void)endwin();
  (void)showGplShort();
  
  (void)fprintf(stderr,"\n");
  (void)fprintf(stderr,"\n");
  (void)fprintf(stderr,"=========================================================================\n");
  (void)fprintf(stderr,"\n");
  
  (void)fprintf(stderr,"Final score: %7.7ld, Final level: %2.2d\nFinal rating... ",score,level);
  if (lives>0) {
    (void)fprintf(stderr,"Quitter\n\n");
  } else if(score<5000) {
    (void)fprintf(stderr,"Alien Fodder\n\n");
  } else if(score<7500) {
    (void)fprintf(stderr,"Easy Target\n\n");
  } else if(score<10000) {
    (void)fprintf(stderr,"Barely Mediocre\n\n");
  } else if(score<12500) {
    (void)fprintf(stderr,"Shows Promise\n\n");
  }else if(score<15000) {
    (void)fprintf(stderr,"Alien Blaster\n\n");
  }else if(score<20000){
    (void)fprintf(stderr,"Earth Defender\n\n");
  }else if(score>19999){
    (void)fprintf(stderr,"Supreme Protector\n\n");
  }else{
    (void)fprintf(stderr, "Error");
  }
  
  (void)showVersion();
  exit(0);
}

static void readInputGamePaused(int32_t ch) 
{
  if (ch == 'p') {
    status = GAME_LOOP;
  }
}

static void readInputGameHighScore(int32_t ch)
{
  if (ch == ' ') {
    titleScreenClear();
    level = 0;      // reset level
    score = 0;      // reset score
    lives = 3;      // restore lives
    status = GAME_NEXTLEVEL;
  } else if (ch == 'q') {	// quit game
    status = GAME_EXIT;
  } else {
    (void)fprintf(stderr, "Error");
  }
}

static void readInputDefault(int32_t ch, int32_t lastmove)
{
  if (ch == 'l' || ch == KEY_RIGHT) {	// move player right
    if (lastmove == 'l') {
      playerTurboOn();	// enable Turbo
    } else {
      playerTurboOff();	// disable Turbo
    }
    playerMoveRight();		// move player
    lastmove = 'l';			// remember last move for turbo mode
  } else if (ch == 'h' || ch == KEY_LEFT) {	// move player left 
    if (lastmove == 'h') {
      playerTurboOn();	// enable Turbo
    } else {
      playerTurboOff();	// disable Turbo
    }
    playerMoveLeft();		// move player
    lastmove = 'h';			// remember last move for turbo mode
  } else if (ch == 'k' || ch == ' ') {	// shoot missile
    playerLaunchMissile();
  } else if (ch == 'p') {			// pause game until 'p' pressed again
    // set status to game paused
    status = GAME_PAUSED;
  } else if (ch == 'W') {			// cheat: goto next level
    status = GAME_NEXTLEVEL;
  } else if (ch == 'L') {			// cheat: one more live
    lives++;
    drawscore();
  } else if (ch == 'q') {	// quit game
    status = GAME_EXIT;
  } else {		// disable turbo mode if key is not kept pressed
    lastmove = ' ';
  }
}

/**
 * reads input from keyboard and do action
 */
static void readInput(void)
{
  int32_t ch;
  static int32_t lastmove;

  ch = getch();		// get key pressed

  switch (status) {

    case GAME_PAUSED:
      readInputGamePaused(ch);
      break;
    
    case GAME_HIGHSCORE:
      readInputGameHighScore(ch);
      break;

    case GAME_OVER:
      break; // don't do anything

    default:
      readInputDefault(ch, lastmove);
      break;

  } // switch
  
}

void handleGameNextLevel(int32_t aliens_move_counter, int32_t aliens_shot_counter, int32_t ufo_move_counter) {
  level+=1;	// increase level

  initLevel();	// initialize level
  
  aliens_move_counter = 0; 
  aliens_shot_counter = 0;
  static int32_t layer_shot_counter = 0;
  ufo_move_counter = 0;
  
  weite = (shipnum+(skill_level*10)-(level*5)+5)/10;
  
  if (weite < 0) {
    weite = 0;
  }
  
  // change status and start game!
  status = GAME_LOOP;
}

void handleGameLoop(int32_t aliens_move_counter, int32_t player_shot_counter, int32_t aliens_shot_counter, int32_t ufo_move_counter) {
  // move aliens			
  if (aliens_move_counter == 0 && aliensMove() == 1) {
    // aliens reached player
    lives = 0;
    status = GAME_OVER;
  }
  
  // move player missile			
  if (player_shot_counter == 0 && playerMoveMissile() == 1) {
    // no aliens left
    status = GAME_NEXTLEVEL;
  }
  
  // move aliens' missiles
  if (aliens_shot_counter == 0 && aliensMissileMove() == 1) {
    // player was hit
    lives--;			// player looses one life
    drawscore();	                // draw score
    playerExplode();		// display some explosion graphics
    if (lives == 0) {		// if no lives left ...
      status = GAME_OVER;		// ... exit game
    }
  }
  
  // move ufo
  if (ufo_move_counter == 0 && ufoShowUfo() == 1) {
    ufoMoveLeft();			// move it one position to the left
  }
  
  
  if (aliens_shot_counter+1 >= 5) {aliens_shot_counter=0;}     // speed of alien shot
  if (player_shot_counter+1 >= 1) {player_shot_counter=0;}     // speed of player shot
  if (aliens_move_counter+1 >= weite) {aliens_move_counter=0;} // speed of aliend
  if (ufo_move_counter+1 >= 3) {ufo_move_counter=0;}           // speed of ufo
  
  refreshScreen();
}

void handleGameOver(int32_t game_over_counter) {
  if (game_over_counter == 100) {
    battleFieldClear();
    status = GAME_HIGHSCORE;
    game_over_counter = 0;
  } else {
    gameOverDisplay();
    game_over_counter++;
  }
}

void handleGameHighScore(int32_t title_animation_counter) {
  if (title_animation_counter == 0) {
    titleScreenDisplay();
  }

  if (title_animation_counter+1 >= 6) {title_animation_counter = 0;} // speed of animation
}

/**
 * timer
 * this method is executed every 1 / FPS seconds  
 */
static void handleTimer(void)
{
  static int32_t aliens_move_counter = 0; 
  static int32_t aliens_shot_counter = 0;
  static int32_t player_shot_counter = 0;
  static int32_t ufo_move_counter = 0;
  static int32_t title_animation_counter = 0;
  static int32_t game_over_counter = 0;
  
  switch (status) {
    case GAME_NEXTLEVEL:    // go to next level
      handleGameNextLevel(aliens_move_counter, aliens_shot_counter, ufo_move_counter);
      break;
    case GAME_LOOP:   	 // do game handling
      handleGameLoop(aliens_move_counter, player_shot_counter, aliens_shot_counter, ufo_move_counter);
      break;
    case GAME_PAUSED:    // game is paused
      break;
    case GAME_OVER:      // game over
      handleGameOver(game_over_counter);
      break;
    case GAME_EXIT:      // exit game
      finish();
      break;
    case GAME_HIGHSCORE: // display highscore
      handleGameHighScore(title_animation_counter);
      break;
    
    default:
      break;
      
  }
}


/**
 * set up timer
 */
static void setUpTimer(void)
{
  struct itimerval myTimer;
  struct sigaction myAction;
  myTimer.it_value.tv_sec = 0;
  myTimer.it_value.tv_usec = 1000000 / FPS;
  myTimer.it_interval.tv_sec = 0;
  myTimer.it_interval.tv_usec = 1000000 / FPS;
  (void)setitimer(ITIMER_REAL, &myTimer, NULL);
  
  myAction.sa_handler = &handleTimer;
  myAction.sa_flags = SA_RESTART;
  (void)sigaction(SIGALRM, &myAction, NULL);
}


int32_t main(int32_t argc, char **argv)
{
  weite = 0;
  score = 0;
  lives = 3;
  level = 0;
  skill_level = 1;

  evaluateCommandLine(argc, argv);	// evaluate command line parameters
  graphicEngineInit();			// initialize graphic engine
  
  // set up timer/ game handling
  setUpTimer();		
  status = GAME_HIGHSCORE;

  // read keyboard input
  do {
    // do movements and key-checking
    readInput();
  } while (0 == 0);
  
  return 0;
}


void doScoring(int32_t alienType)
{
  int32_t points[4] = {500, 200, 150, 100};   	// 0: ufo, 1:red, 2:green, 3:blue
  
  score += points[alienType];		// every alien type does different scoring points
  
  // every 6000 pts player gets a new live
  if (score % 6000 == 0){
    lives++;
  }
  
  drawscore();	// display score
}

