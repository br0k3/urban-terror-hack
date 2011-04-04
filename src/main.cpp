/*
	urth - Urban Terror Hack
	http://www.nixcoders.org/
	Copyright (C) 2007 - *nixCoders team

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

#include "main.h"

// Main objects instance
Urt *urt = NULL;
UIGame *uigame = NULL;
CGame *cgame = NULL;

LOG_FILE_DEF;

// Forward declaration
bool isCurrentProcess(char *name);

__attribute__ ((constructor))
void urthStart() {
	std::cout << "urth v" URTH_VERSION " started" << std::endl;

	// Search urt binary
	if (isCurrentProcess(URT_BINARY_NAME)) {
		LOG_FILE_INIT;
		// LOG_DEBUG("found " URT_BINARY_NAME " pid " << getpid());

		urt = new Urt();
	}
}

// Helper since it's used in both constructor and destructor
bool isCurrentProcess(char *name) {
	char exeFilename[PATH_MAX];
	memset(exeFilename, 0, sizeof(exeFilename));

	if (readlink(PROC_SELF_EXE, exeFilename, sizeof(exeFilename)) <= 0) {
		perror("fatal error: Can't find " PROC_SELF_EXE);
		exit(1);
	}

	return !strcmp(basename(exeFilename), name);
}
