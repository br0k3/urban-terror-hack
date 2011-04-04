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

#include "ghf.h"

#include <stdio.h>
#include <zlib.h>

unsigned long crc32FromFile(const char* filename) {
	FILE *file;
	
	if ((file = fopen(filename, "rb")) == NULL)
		return 0;
	
	unsigned char buffer[512];
	int length;
	unsigned long crc = crc32(0L, Z_NULL, 0);

	while ((length = fread(buffer, 1, sizeof(buffer), file)) != 0)
		crc = crc32(crc, buffer, length);

	fclose(file);
	return crc;
}
