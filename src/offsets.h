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

#ifndef OFFSETS_H_
#define OFFSETS_H_

// Can be use a day:
// Qvm cg_numSolidEntities 0x38d274
// Qvm CG_Trace 0x413a6
// JIT CG_Trace 0xaa135


const static urtQvm_t urtQvms[] = {
	{
		"4.1",		// version
		CGAME_QVM,	// type
		0x0,		// crc32
		
		0x31df64,	// cg
		0x72f64,	// cg_entities
		0xaac		// sizeof(centity_t) - old offset - 0xaac - new - 0x2d8
	}
};

#endif /*OFFSETS_H_*/
