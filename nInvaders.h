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

#ifndef NINVADERS
#define NINVADERS


#define KEY_RIGHT 67
#define KEY_LEFT 68

#define UFO_ALIEN_TYPE   0

int32_t main(int32_t argc, char **argv);	// hey! it has to start somewhere!
		
void doScoring(int32_t alienType);

// todo: let's try to not having to declare these "public"
extern int32_t weite;
extern int32_t level;
extern int32_t skill_level;

#endif
