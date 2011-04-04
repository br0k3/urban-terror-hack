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

#ifndef MAIN_H_
#define MAIN_H_

#include <iostream>
#include <csignal>
#include <fstream>
#include <vector>

extern "C" {
	#include "ioq3/code/qcommon/vm_local.h"
	#include "ioq3/code/client/client.h"
	#include "ioq3/code/cgame/cg_local.h"
	#include "ioq3/code/renderer/tr_local.h"
}

#include "ghf/ghf.h"

#include "logger.h"
#include "types.h"
#include "offsets.h"
#include "hook.h"
#include "g_functions.h"

#include "Qvm.h"
#include "UIGame.h"
#include "CGame.h"
#include "Urt.h"

#define URT_BINARY_NAME "ioUrbanTerror.i386"
#define URTH_DEFAULT_LOG_FILE "/tmp/urth.log"

extern Urt *urt;
extern UIGame *uigame;
extern CGame *cgame;

#endif /*MAIN_H_*/
