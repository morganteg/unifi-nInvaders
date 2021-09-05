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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 * homepage: ninvaders.sourceforge.net
 * mailto: ninvaders-devel@lists.sourceforge.net
 *
 */

#include "view.h"
#include "aliens.h"
#include "player.h"
#include "nInvaders.h"
#include <stdlib.h>

/**
 * initialize aliens attributes
 */
void aliensReset(void)
{
	int32_t  i,j;
	
	// three different types of aliens [5], [10]
	int32_t alienLevel[ALIENS_MAX_NUMBER_Y][ALIENS_MAX_NUMBER_X]={
		{1,1,1,1,1,1,1,1,1,1},
		{2,2,2,2,2,2,2,2,2,2},
		{2,2,2,2,2,2,2,2,2,2},
		{3,3,3,3,3,3,3,3,3,3},
		{3,3,3,3,3,3,3,3,3,3}
	};

	aliensClear(aliens.posX, aliens.posY, aliens.right, aliens.bottom);	// clear old position of aliens
	
	// reset alien position
	aliens.posX = 0;
	aliens.posY = 0;
	aliens.right = 0;
	aliens.bottom = 0;
	aliens.left = 0;
	aliens.speed = 1;
	
	// copy level-array to enemy-array 
	for (i=0;i<ALIENS_MAX_NUMBER_X;i++) {
		for (j=0;j<ALIENS_MAX_NUMBER_Y;j++) {
			alienBlock[j][i]=alienLevel[j][i];
		}
	}
	
	// reset missiles
	for (i = 0; i < ALIENS_MAX_MISSILES; i++) {
		if (alienshotx[i] != 0) {
			aliensMissileClear(alienshotx[i],alienshoty[i]);	// clear old position
		}
		alienshotx[i] = 0;  // start with zero values
		alienshoty[i] = 0;  // start with zero values
	}
	int32_t alienshotnum = 1;	    // one missile at the same time
	alienshotx[0] = 5;	    // x position of first alienshot
	alienshoty[0] = 1;	    // y position of first alienshot

}

/**
 * initialize bunkers attributes
 */
void bunkersReset(void)
{
	int32_t a, b;

	// set position of bunker sprites. user graphical int8_t bunkerd for better visual overview
	// and set according to this the bunker-array
	int8_t bunkerd[BUNKERHEIGHT][BUNKERWIDTH+1] = {
		"        ###                 ###                 ###                 ###         ",
		"       #####               #####               #####               #####        ",
		"      #######             #######             #######             #######       ",
		"      ##   ##             ##   ##             ##   ##             ##   ##       "
	};
	//       12345678901234567890123456789012345678901234567890123456789012345678901234567890
	// 80 characters wide

	// copy graphical "bunkerd" to binary "bunker"
	for (a = 0; a < BUNKERWIDTH; a++) {
		for (b = 0; b < BUNKERHEIGHT; b++) {
                  if (bunkerd[b][a] == '#') {
				bunker[b][a] = 1;
                  }else{
				bunker[b][a] = 0;
                  }
		}
	}
	
	// display bunkers sprite
	bunkersDisplay(&bunker[0][0]);	
}

/**
 * move aliens and test if they've reached the
 * bottom of the windows or the bunkers.
 */
int32_t aliensMove(void)
{
	
	int32_t cx,cy;
	int32_t fReachedPlayer=0; 				// return value

	aliensClear(aliens.posX, aliens.posY, aliens.right, aliens.bottom);	// clear old position of aliens

	aliens.posX = aliens.posX + aliens.speed;			// move aliens left/ right
	
	// when aliens reached left or right screen-border
	if (aliens.posX == (BUNKERWIDTH + BUNKERX - 5 - aliens.right) || aliens.posX == (BUNKERX + aliens.left)) {
		
		aliens.posY++;				// move aliens downwards
		
		// aliens reached player
		if (aliens.posY == SCREENHEIGHT - 2 - aliens.bottom) {
			fReachedPlayer = 1;		// set return value
		}
		
		// aliens reached bunkers //funzt nicht ganz: todo
		if (aliens.posY == BUNKERY - aliens.bottom) {
			// clear bunkers
			for(cx=0;cx<BUNKERWIDTH;cx++) {
				for(cy=0;cy<BUNKERHEIGHT;cy++) { 
					bunker[cy][cx]=0;	
				}
			}
			bunkersClear();	// clear bunkers sprites
		}
		
		aliens.speed = aliens.speed * (-1);		  // change direction of aliens' movements
	}

	aliensDisplay(aliens.posX, aliens.posY, aliens.right, aliens.bottom); // display aliens at new position
	
	return fReachedPlayer;				  // return if aliens reached player
}

/**
 * move aliens' missiles and do player/bunker hit testing
 * a zero value in alienshotx indicates that the appropriate missile is loaded, but not fired
 */
int32_t aliensMissileMove(void)
{
	int32_t i, tmp;
	int32_t fPlayerWasHit = 0;
	int32_t shootThreshold;
	static int32_t alienshot_counter = 0;

	
	// calculate threshold when next missile should be fired
	// it is done here to save calculation time in for-instruction 
	shootThreshold = (skill_level * 8) * (shipnum + 2);
	alienshot_counter = alienshot_counter + 10 ;
	
	// loop all possible missiles
	for (i = 0; i < ALIENS_MAX_MISSILES; i++) {
		
		// if the current missile is flying we should do movements
		if (alienshotx[i] != 0) {
			
			aliensMissileClear(alienshotx[i],alienshoty[i]);	// clear old position
				
			// if missile hit the bunkers	
			if (bunkersHitCheck(alienshotx[i], alienshoty[i]) == 1) {
				alienshotx[i] = 0;		// value of zero reloads missile
			}
			
			alienshoty[i]++;			// move missile downwards
			
			// check if player was hit by an alien missile
			if (playerHitCheck(alienshotx[i], alienshoty[i]) == 1) {
				alienshotx[i] = 0;		// value of zero reloads missile
				fPlayerWasHit = 1;
			}
			
			
		} else {					// missile not launched yet
			
			// start new missile if counter says so
			if (alienshot_counter > shootThreshold && shipnum > 0) {// only shot if there's an alien left
				alienshot_counter = 0;				// reset counter				
				tmp = rand() % ALIENS_MAX_NUMBER_X;  		// randomly select one of the ...
				while (lowest_ship[tmp] == -1) {		// ...aliens at the bottom of ...
					tmp = rand() % ALIENS_MAX_NUMBER_X;	// ...a column to launch missile
				}
				alienshoty[i]=aliens.posY+lowest_ship[tmp];		// set y position of missile
				alienshotx[i]=aliens.posX+tmp*3;			// set x position of missile
			}
		} // if 
		
		// display missiles if still running or just launched; could have been modified in the above code
		if (alienshotx[i] != 0) {
			// if missile is out of battlefield 
			if (alienshoty[i] == SCREENHEIGHT - 1) {
				alienshotx[i] = 0;	// reload missile	
			} else {
				aliensMissileDisplay(alienshotx[i], alienshoty[i]); // display Missile at new position
			}
		}		
		
	} // for


	return fPlayerWasHit;
}



/**
 * check if missile hit an alien
 */
int32_t aliensHitCheck(int32_t shotx, int32_t shoty)
{
	int32_t alienType = 0;
	int32_t shipx, shipy;
	// if missile is within alien-rectangle 
	if (shotx >= aliens.posX && shotx <= aliens.posX + ALIENS_MAX_NUMBER_X * 3 - 1
	    && shoty >= aliens.posY && shoty <= aliens.posY + (ALIENS_MAX_NUMBER_Y - 1) * 2) {
		// calculate the ship that was hit
		shipx = (shotx - aliens.posX) / 3;
		shipy = (shoty - aliens.posY) / 2;
		// if there is still a ship at this position
		alienType = alienBlock[shipy][shipx];
		if (alienType != 0) {
			alienBlock[shipy][shipx] = 0;	// delete alien ship
		}
	}
	return alienType; 	// returns 0 if no alien was hit, else returns type-code of alien
}

/**
 * check if missile hit an element of bunker
 */
int32_t bunkersHitCheck(int32_t shotx, int32_t shoty)
{
	int32_t adjx, adjy;
	int32_t fBunkerWasHit = 0;
	// if missile is within bunker-rectangle
	if (shotx >= BUNKERX && shotx < BUNKERX + BUNKERWIDTH
	    && shoty >= BUNKERY && shoty < BUNKERY + BUNKERHEIGHT) {
		// calculate the element of the bunker that was hit
		adjy = shoty - BUNKERY; 
		adjx = shotx - BUNKERX;
		// if there is still an element
		if(bunker[adjy][adjx] == 1){
			bunker[adjy][adjx] = 0;	// delete element
			fBunkerWasHit = 1; 		// bunker was hit!
		}
	}
	return fBunkerWasHit;
}
