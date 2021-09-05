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
#ifndef VIEW
#define VIEW

#include "globals.h"
#include <stdint.h>

#define SCREENHEIGHT 24
#define SCREENWIDTH 80	
	
#define PLAYERWIDTH 5
#define PLAYERPOSY (SCREENHEIGHT-2)

#define BUNKERWIDTH 80
#define BUNKERHEIGHT 4
#define BUNKERX 0
#define BUNKERY (SCREENHEIGHT-8)
	
#define ALIENS_MAX_NUMBER_X 10
#define ALIENS_MAX_NUMBER_Y 5	

#define UFOWIDTH 5
#define UFOPOSY 0

	
void graphicEngineInit(void);
	
void aliensClear(int32_t x, int32_t y, int32_t wid, int32_t hgt);
void aliensDisplay(int32_t x, int32_t y, int32_t wid, int32_t hgt);
void aliensMissileClear(int32_t x, int32_t y);
void aliensMissileDisplay(int32_t x, int32_t y);
void aliensRefresh(int32_t levelView, int32_t *pAliens);
void battleFieldClear(void);
void bunkersClear(void);
void bunkersClearElement(int32_t x, int32_t y);
void bunkersDisplay(int32_t *pBunker);
void gameOverDisplay(void);
void playerClear(int32_t x, int32_t y);
void playerDisplay(int32_t x, int32_t y);
void playerExplosionDisplay(int32_t x, int32_t y);
void playerMissileClear(int32_t x, int32_t y);
void playerMissileDisplay(int32_t x, int32_t y);
void titleScreenClear(void);
void titleScreenDisplay(void);
void ufoClear(int32_t x, int32_t y);
void ufoDisplay(int32_t x, int32_t y);
void ufoRefresh(void);

void statusDisplay(int32_t levelView, int32_t scoreView, int32_t livesView);	
void refreshScreen(void);

#endif
