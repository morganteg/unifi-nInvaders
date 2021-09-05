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


#ifndef MACRO_ALIENS
#define MACRO_ALIENS
	
extern struct AliensStruct {
	int32_t posX;	  // horizontal position of aliens
	int32_t posY;	  // vertical position of aliens
	int32_t right;
	int32_t left;
	int32_t bottom;
	int32_t speed;	  // 0: no movement; 1: one per turn; etc.
};
	
extern struct AliensStruct aliens;
		
extern int32_t shipnum;	

#define ALIENS_MAX_NUMBER_X 10
#define ALIENS_MAX_NUMBER_Y 5	
#define ALIENS_MAX_MISSILES 10	

// todo: move to structure
extern int32_t lowest_ship[ALIENS_MAX_NUMBER_X];
extern int32_t alienshotx[ALIENS_MAX_MISSILES];
extern int32_t alienshoty[ALIENS_MAX_MISSILES];
extern int32_t alienBlock[ALIENS_MAX_NUMBER_Y][ALIENS_MAX_NUMBER_X];

extern int32_t bunker[BUNKERHEIGHT][BUNKERWIDTH + 1];	


void aliensReset(void);
void bunkersReset(void);	
int32_t aliensMove(void);
int32_t aliensMissileMove(void);
int32_t aliensHitCheck(int32_t shotx, int32_t shoty);
extern int32_t bunkersHitCheck(int32_t shotx, int32_t shoty);

#endif
