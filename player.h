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
 

#ifndef MACRO_PLAYER
#define MACRO_PLAYER

#include "view.h"
#include <stdint.h>


void playerReset(void);
	
void playerMoveLeft(void);
void playerMoveRight(void);
void playerTurboOn(void);
void playerTurboOff(void);
int32_t playerHitCheck(int32_t hostileShotX, int32_t hostileShotY);
void playerLaunchMissile(void);
int32_t playerMoveMissile(void);
void playerExplode(void);
	
#endif

