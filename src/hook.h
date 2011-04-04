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

#ifndef HOOK_H_
#define HOOK_H_

#define PROC_SELF_EXE "/proc/self/exe"

VM_LoadQVM_t urth_VM_LoadQVM;
VM_Free_t urth_VM_Free;
VM_CallCompiled_t urth_VM_CallCompiled;

RE_RenderScene_t urth_RE_RenderScene;
CL_CgameSystemCalls_t urth_CL_CgameSystemCalls;

#endif /*HOOK_H_*/
