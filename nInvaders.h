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
 

#include <stdio.h>
#include <stdlib.h>
#include "view.h"
#include "globals.h"


#ifndef NINVADERS
#define NINVADERS

#define UFO_ALIEN_TYPE   0

int main(int argv, char **argc);	// hey! it has to start somewhere!

extern void render(void);
	
void game_over(int a);
static void drawscore(void);
static void readInput(void);
		
void doScoring(int alienType);

// todo: let's try to not having to declare these "public"
int weite;
int level;
int skill_level;
	
// included from globals.h
extern void doSleep(int microseconds);
extern void showUsage(void);
extern void showVersion(void);
extern void showGplShort(void);
extern void showGpl(void);

#endif
