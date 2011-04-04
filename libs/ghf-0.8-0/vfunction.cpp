/*
	ghf - Game Hack Framework for Linux
	http://www.nixcoders.org/
	Copyright (C) 2008 - *nixCoders team

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2, or (at your option)
	any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software Foundation,
	Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "ghf.h"

// Return vtable index of a virtual function
int getVTblIndex(const char *fmt, ...) {
	va_list va_alist;
	char buf[8];
	memset(buf, 0, sizeof(buf));
	
	va_start(va_alist, fmt);
		vsnprintf(buf, sizeof(buf), fmt, va_alist);
	va_end(va_alist);

	return (strtoul(buf, NULL, 16) - 1) / 4;
}
