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
 
#include <ncurses.h>
#include "view.h"
#include <stdlib.h>	
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

#define RED 1
#define GREEN 2
#define YELLOW 3
#define BLUE 4
#define CYAN 5
#define MAGENTA 6
#define WHITE 7

static WINDOW *wBattleField;
static WINDOW *wEmpty;
static WINDOW *wScores;	

static WINDOW *wPlayer;
static WINDOW *wPlayerMissile;
static WINDOW *wAliens;
static WINDOW *wAliensMissile;	
static WINDOW *wBunkers;
static WINDOW *wGameOver;
static WINDOW *wUfo;
static WINDOW *wStatus;
static WINDOW *wTitleScreen;

/**
 * initialize player sprites
 */
static void playerInit(void)
{
	wPlayer = newpad(1, PLAYERWIDTH);       // new pad with appropriate size
	(void)wclear(wPlayer);			// clear pad
        (void)wattrset(wPlayer,COLOR_PAIR(YELLOW));	// set color
        waddstr(wPlayer,"/-^-\\");	        // set sprite
}


/**
 * display player sprite
 */
void playerDisplay(int32_t x, int32_t y) 
{
	(void)copywin(wPlayer,wBattleField,0,0,y,x,y,x+PLAYERWIDTH-1,0);
}


/**
 * clear player sprite
 */
void playerClear(int32_t x, int32_t y) 
{
	(void)copywin(wEmpty,wBattleField,0,0,y,x,y,x+PLAYERWIDTH-1,0);
}


/**
 * initialize missile sprites
 */
static void playerMissileInit(void)
{
	wPlayerMissile = newpad(1, 1);		// new pad with appropriate size
	(void)wclear(wPlayerMissile);			// clear pad
	(void)wattrset(wPlayerMissile,COLOR_PAIR(WHITE));	// set color
	(void)waddch(wPlayerMissile,'!');		// set sprite
	(void)wrefresh(wPlayerMissile);
}


/**
 * display missile sprite
 */
void playerMissileDisplay(int32_t x, int32_t y) 
{
	(void)copywin(wPlayerMissile,wBattleField,0,0,y,x,y,x,0);
}


/**
 * clear missile sprite
 */
void playerMissileClear(int32_t x, int32_t y) 
{
	(void)copywin(wEmpty,wBattleField,0,0,y,x,y,x,0);
}


/**
 * some explosion animation
 */
void playerExplosionDisplay(int32_t x, int32_t y)
{
	WINDOW* wPlayerExplosion;
	int8_t playerExplosionChars[16+1]="@~`.,^#*-_=\\/%{}";
	int32_t t,s;
	
	wPlayerExplosion=newpad(1,PLAYERWIDTH);		// new pad
	(void)wattrset(wPlayerExplosion,COLOR_PAIR(YELLOW));	// set color
	
	for(t=0;t<5;t++){ 			// 5 frames
		(void)wclear(wPlayerExplosion);	// clear pad
		for(s=0;s<PLAYERWIDTH;s++){
			(void)waddch(wPlayerExplosion,playerExplosionChars[rand()%16]);	// sprite
		}

		(void)copywin(wPlayerExplosion,wBattleField,0,0,y,x,y,x+PLAYERWIDTH-1,0); 	// display explostion
		(void)wrefresh(wBattleField); 	// refresh battelfield to display explosion frames
		doSleep(100000);		// play animation not too fast
	}
	

} // todo: kann man bestimmt noch besser machen.


/**
 * initialize aliens sprites
 */
static void aliensInit(void)
{
	wAliens = newpad(ALIENS_MAX_NUMBER_Y*2,ALIENS_MAX_NUMBER_X*3);
	(void)wclear(wAliens);
}


/**
 * display aliens sprite
 */
void aliensDisplay(int32_t x, int32_t y, int32_t wid, int32_t hgt) 
{
	(void)copywin(wAliens,wBattleField,0,0,y,x,y+hgt,x+wid+2,0);
}


/**
 * clear aliens sprite
 */
void aliensClear(int32_t x, int32_t y, int32_t wid, int32_t hgt) 
{
	(void)copywin(wEmpty,wBattleField,0,0,y,x,y+hgt,x+wid+2,0);
}


/**
 * initialize missile sprites
 */
static void aliensMissileInit(void)
{
	wAliensMissile = newpad(1, 1);		// new pad
	(void)wclear(wAliensMissile);			// clear pad
	(void)wattrset(wAliensMissile, COLOR_PAIR(CYAN));	// set color
	(void)waddch(wAliensMissile, ':');			// set sprite
}


/**
 * display missile sprite
 */
void aliensMissileDisplay(int32_t x, int32_t y) 
{
	(void)copywin(wAliensMissile,wBattleField,0,0,y,x,y,x,0);
}


/**
 * clear missile sprite
 */
void aliensMissileClear(int32_t x, int32_t y) 
{
	(void)copywin(wEmpty,wBattleField,0,0,y,x,y,x,0);
}


/**
 * refresh aliens sprite
 */
void aliensRefresh(int32_t levelView, int32_t *pAliens) 
{
	static int32_t frame = 0; // used for animation; mod 2 == 0: frame1, mod2 == 1: frame2
	int32_t k,row;
	int32_t c = 0;
	int32_t alienType = 0;
	int8_t ships[2][9][3+1] = {
		{",^,", "_O-", "-o-",  "o=o", "<O>", "_x_", "*^*", "\\_/", "o o"},
		{".-.", "-O_", "/o\\", "o-o", "<o>", "-x-", "o^o", "/~\\", "oo "}
	};
	int32_t colors[9] = {RED, GREEN, BLUE, RED, YELLOW, WHITE, WHITE, YELLOW, RED};

	(void)wclear(wAliens);						// clear pad
	(void)wattrset(wAliens,COLOR_PAIR(RED));				// set color
	
	frame++;						// next frame
	
	// draw alien if there is one
	for (row = 0; row < ALIENS_MAX_NUMBER_Y*2; row++) {			
		for (k = 0; k < ALIENS_MAX_NUMBER_X; k++) {
			if ((row % 2) == 0) {			// display aliens every even row
				alienType = *(pAliens + c * (ALIENS_MAX_NUMBER_X) + k); 	// get type of alien //alienBlock[c][k]
				
				if (alienType != 0) {		// if there is an alien to display
					(void)wattrset(wAliens,COLOR_PAIR(colors[alienType-1]));		   // set color
					(void)waddch(wAliens,ships[frame%2][alienType-1+(3*((levelView-1)%3))][0]);  // set char1
					(void)waddch(wAliens,ships[frame%2][alienType-1+(3*((levelView-1)%3))][1]);  // set char2
					(void)waddch(wAliens,ships[frame%2][alienType-1+(3*((levelView-1)%3))][2]);  // set char3
					if (alienType > 4) {
						*(pAliens + c * ALIENS_MAX_NUMBER_X + k) = (alienType + 1) % 9;
					} // todo: what's that? If alien_type > 4 then do a modulo operation???
				} else {
					waddstr(wAliens,"   ");	// no alien
				}
				
			} else {
				waddstr(wAliens,"   ");		// no aliens at odd rows
			}
		}
		if ((row % 2) == 1) {c++;} // goto next row at alienblock
	}
}


/**
 * initialize bunkers sprites
 */
static void bunkersInit(void)
{
	wBunkers = newpad(BUNKERHEIGHT, BUNKERWIDTH);		// new pad data
	(void)wclear(wBunkers);
}


/**
 * display bunkers sprite
 * needs pointer to bunker-array
 */
void bunkersDisplay(int32_t *pBunker) 
{
	int32_t l, k;
	(void)wclear(wBunkers);
	(void)wattrset(wBunkers,COLOR_PAIR(CYAN));
	for (l=0;l<BUNKERHEIGHT;l++) {
		for (k=0;k<BUNKERWIDTH;k++) {
			if (*(pBunker + (l * (BUNKERWIDTH + 1)) + k) == 1) {
				(void)waddch(wBunkers,'#');
			} else {
				(void)waddch(wBunkers,' ');
			}
		}	
	}
	
	(void)copywin(wBunkers, wBattleField, 0, 0, BUNKERY, BUNKERX, BUNKERY + BUNKERHEIGHT - 1, BUNKERX + BUNKERWIDTH - 1, 0);
}


/**
 * clear bunkers sprite
 */
void bunkersClear(void) 
{
	(void)copywin(wEmpty, wBattleField, 0, 0, BUNKERY, BUNKERX, BUNKERY + BUNKERHEIGHT - 1, BUNKERX + BUNKERWIDTH - 1, 0);
}


/**
 * clear one element of bunkers sprite at position (x, y)
 */
void bunkersClearElement(int32_t x, int32_t y) 
{
	(void)copywin(wEmpty, wBattleField, 0, 0, y, x, y, x, 0);
}


/**
 * set actual sprite for ufo animation
 */
void ufoRefresh(void)
{
	int8_t ufo[4][6] = {"<o o>", "<oo >", "<o o>", "< oo>"};
	static int32_t frame = 0;

	(void)wclear(wUfo);
        (void)wattrset(wUfo, COLOR_PAIR(MAGENTA));
	waddstr(wUfo, ufo[frame % 4]);

	frame++;
}


/**
 * initialize ufo sprite
 */
static void ufoInit(void)
{
	wUfo = newpad(1, UFOWIDTH);	     // new pad with appropriate size
	(void)wclear(wUfo);    		     // clear pad
        (void)wattrset(wUfo, COLOR_PAIR(MAGENTA)); // set color
}


/**
 * display ufo sprite
 */
void ufoDisplay(int32_t x, int32_t y)
{
	(void)copywin(wUfo, wBattleField, 0, 0, y, x, y, x + UFOWIDTH - 1, 0);
}


/**
 * clear ufo sprite
 */
void ufoClear(int32_t x, int32_t y) 
{
	(void)copywin(wEmpty, wBattleField, 0, 0, y, x, y, x + UFOWIDTH - 1, 0);
}


/**
 * initialize gameover graphic
 */
static void gameOverInit(void)
{
	// init game-over banner
	wGameOver = newpad(13, 31);
	(void)wclear(wGameOver);
	(void)wattrset(wGameOver, COLOR_PAIR(WHITE));
	waddstr(wGameOver, "                               ");
	waddstr(wGameOver, "  #####   ####  ##   ## ###### ");
	waddstr(wGameOver, " ##      ##  ## ####### ##     ");
	waddstr(wGameOver, " ## ###  ###### ## # ## #####  ");
	waddstr(wGameOver, " ##  ##  ##  ## ##   ## ##     ");
	waddstr(wGameOver, "  #####  ##  ## ##   ## ###### ");
	waddstr(wGameOver, "                               ");
	waddstr(wGameOver, "  ####  ##   ## ###### ######  ");
	waddstr(wGameOver, " ##  ## ##   ## ##     ##   ## ");
	waddstr(wGameOver, " ##  ##  ## ##  #####  ######  ");
	waddstr(wGameOver, " ##  ##  ## ##  ##     ##  ##  ");
	waddstr(wGameOver, "  ####    ###   ###### ##   ## ");
	waddstr(wGameOver, "                               ");
}

/**
 * display game over graphic
 */ 
void gameOverDisplay(void)
{
	int32_t x = (SCREENWIDTH / 2) - (31 / 2);
	int32_t y = (SCREENHEIGHT / 2) - (13 / 2);
	(void)copywin(wGameOver, wBattleField, 0, 0, y, x, y + 12, x + 30, 0);
	(void)wrefresh(wBattleField);
}


/**
 * initialize title screen
 */
static void titleScreenInit(void)
{
	wTitleScreen = newpad(SCREENHEIGHT, SCREENWIDTH);
	(void)wclear(wTitleScreen);
}


/**
 * display title screen
 */
void titleScreenDisplay(void)
{
	static int32_t frame = 0;
	int32_t x, y;
	int32_t iCounter;
	WINDOW *wTitleText;
	WINDOW *wAliensLocal;
	WINDOW *wStartText;
	int8_t ufo[4][6] = {"<o o>", "<oo >", "<o o>", "< oo>"};
	int8_t aliensView[2][9][3+1] = {
		{",^,", "_O-", "-o-",  "o=o", "<O>", "_x_", "*^*", "\\_/", "o o"},
		{".-.", "-O_", "/o\\", "o-o", "<o>", "-x-", "o^o", "/~\\", "oo "}
	};
	int32_t scoreView[3] = {200, 150, 100};
	int32_t colors[9] = {RED, GREEN, BLUE, RED, GREEN, BLUE, RED, GREEN, BLUE};
	int8_t buffer[12];
	static int32_t alienTypeView = 0;

	wTitleText = newpad(4, 41);
	(void)wclear(wTitleText);
	(void)wattrset(wTitleText, COLOR_PAIR(YELLOW));
	waddstr(wTitleText, "        ____                 __          ");
	waddstr(wTitleText, "  ___  /  _/__ _  _____  ___/ /__ _______");
        waddstr(wTitleText, " / _ \\_/ // _ \\ |/ / _ `/ _  / -_) __(_-<");
	waddstr(wTitleText, "/_//_/___/_//_/___/\\_,_/\\_,_/\\__/_/ /___/");

	frame++;
	wAliensLocal = newpad(7, 11);
	(void)wclear(wAliensLocal);
	(void)snprintf(buffer, sizeof(buffer),"%s = 500", ufo[frame % 4]);
	(void)wattrset(wAliensLocal, COLOR_PAIR(MAGENTA));
	waddstr(wAliensLocal, buffer);
	if ((frame = frame % 60) == 0) {
		alienTypeView = 0;
	} else if (frame == 20) {
		alienTypeView = 3;
	} else if (frame == 40) {
		alienTypeView = 6;
	}
	for (iCounter = alienTypeView; iCounter < alienTypeView + 3; iCounter++) {
		waddstr(wAliensLocal, "           ");
		(void)snprintf(buffer, sizeof(buffer), "%s   = %d", aliensView[frame % 2][iCounter], scoreView[iCounter % 3]);
		(void)wattrset(wAliensLocal, COLOR_PAIR(colors[iCounter]));
		waddstr(wAliensLocal, buffer);
	}

	wStartText = newpad(1, 20);
	(void)wclear(wStartText);
	(void)wattrset(wStartText, COLOR_PAIR(RED));
	waddstr(wStartText, "Press SPACE to start");
	
	x = (SCREENWIDTH / 2) - (41 / 2);
	y = 0;
	(void)copywin(wTitleText, wTitleScreen, 0, 0, y, x, y + 3, x + 40, 0);

	x = (SCREENWIDTH / 2) - (11 / 2);
	y = 8;
	(void)copywin(wAliensLocal, wTitleScreen, 0, 0, y, x , y + 6, x + 10, 0);

	x = (SCREENWIDTH / 2) - (20 / 2);
	y = SCREENHEIGHT - 2;
	(void)copywin(wStartText, wTitleScreen, 0, 0, y, x, y, x + 19, 0);
	
	(void)copywin(wTitleScreen, wBattleField, 0, 0, 0, 0, SCREENHEIGHT-1, SCREENWIDTH-1, 0);
	
	(void)wrefresh(wBattleField);
}


/**
 * clear title screen
 */
void titleScreenClear(void)
{
	(void)battleFieldClear();
}


/**
 * initialize scores
 */
static void statusInit(void)
{
	wStatus = newpad(1, 55);
	(void)wclear(wStatus);
}


/**
 * display scores
 */
void statusDisplay(int32_t levelView, int32_t scoreView, int32_t livesView)
{
	int32_t t, xOffset;
	int8_t strStatus[55];
	// "Level: 01 Score: 0001450 Lives: /-\ /-\ /-\ /-\ /-\ "
	// "1234567890123456789012345678901234567890123456789012"

	
	xOffset = (SCREENWIDTH / 2) - 24;
	


	(void)sprintf(strStatus, "Level: %2.2d Score: %2.7d Lives: ", levelView, scoreView);

	(void)wclear(wStatus);
	(void)wattrset(wStatus, COLOR_PAIR(RED));
	waddstr(wStatus, strStatus);

	// show maximal five lives
	for (t = 1; ((t <= 5) && (t < livesView)); t++){
		waddstr(wStatus, "/-\\ ");
	}
	
	(void)copywin(wStatus, wBattleField, 0, 0, SCREENHEIGHT-1, xOffset, SCREENHEIGHT-1, xOffset + 54, 0);
	

}


/**
 * initialize battlefield
 */
static void battleFieldInit(void)
{
	wEmpty = newpad(SCREENHEIGHT, SCREENWIDTH);
	(void)wclear(wEmpty);
	
	wBattleField = newwin(SCREENHEIGHT, SCREENWIDTH, 0, 0);	// new window
	(void)wclear(wBattleField);						// clear window
	(void)mvwin(wBattleField, 0, 0);					// move window
}


/**
 * clear battlefield
 */
void battleFieldClear(void)
{
	(void)copywin(wEmpty,wBattleField,0,0,0,0,SCREENHEIGHT-1,SCREENWIDTH-1,0);
}


/**
 * refresh screen so that modified graphic buffers get visible
 */
void refreshScreen(void)
{
	redrawwin(wBattleField); // needed to display graphics properly at startup on some terminals
	(void)wrefresh(wBattleField);

}	


/**
 * do proper cleanup
 */
static void finishView(void)
{
	(void)endwin();	// <curses.h> reset terminal into proper non-visual mode
	exit(0);
}


/**
 * initialize n_courses
 */
void graphicEngineInit(void)
{
	(void) signal(SIGINT, finishView);	// <signal.h> on signal "SIGINT" call method "finish"
	(void) initscr();		// <curses.h> do initialization work 
	(void)keypad(stdscr, TRUE);		// <curses.h> enable keypad for input
	(void) nonl();			// <curses.h> disable translation return/ newline for detection of return key
	(void) cbreak();		// <curses.h> do not buffer typed characters, use at once
	(void) noecho();		// <curses.h> do not echo typed characters
	(void)start_color();			// <curses.h> use colors
	(void)init_pair(RED, COLOR_RED, COLOR_BLACK);		// <curses.h> define color-pair
	(void)init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);	// <curses.h> define color-pair
	(void)init_pair(YELLOW, COLOR_YELLOW, COLOR_BLACK);	// <curses.h> define color-pair
	(void)init_pair(BLUE, COLOR_BLUE, COLOR_BLACK);	// <curses.h> define color-pair
	(void)init_pair(CYAN, COLOR_CYAN, COLOR_BLACK);	// <curses.h> define color-pair
	(void)init_pair(MAGENTA, COLOR_MAGENTA, COLOR_BLACK);	// <curses.h> define color-pair
	(void)init_pair(WHITE, COLOR_WHITE, COLOR_BLACK);	// <curses.h> define color-pair
	
	//timeout(0); 			// <curses.h> do not wait for input

	// initialize sprites 
	battleFieldInit();
	playerInit();
	playerMissileInit();
	aliensInit();
	aliensMissileInit();
	bunkersInit();
	ufoInit();
	gameOverInit();
	statusInit();
	titleScreenInit();
}

