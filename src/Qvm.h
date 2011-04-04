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

#ifndef QVM_H_
#define QVM_H_

#define CGAME_QVM_NAME "cgame"
#define UIGAME_QVM_NAME "ui"

class Qvm {
public:
	Qvm(vm_t *vm);
	virtual ~Qvm();

	vm_t *vm;

	void *VM_ArgPtr(intptr_t qvmOffset);

protected:
	const urtQvm_t *qvm;
};

#endif /*QVM_H_*/
