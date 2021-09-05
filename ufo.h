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



#ifndef MACRO_UFO
#define MACRO_UFO
		
struct UfoStruct {
	int32_t posX;	  // horizontal position of aliens
	int32_t posY;	  // vertical position of aliens
};
	
extern struct UfoStruct ufoVar;

void ufoReset(void);
int32_t ufoShowUfo(void);
void ufoMoveLeft(void);
int32_t ufoHitCheck(int32_t shotCoordX, int32_t shotCoordY);

#endif
