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

#ifndef GHF_H_
#define GHF_H_

#include <gelf.h>
#include <sys/mman.h>
#include <errno.h>

// Helper macro
#define GET_PAGE(addr) ((void *)(((uintptr_t)addr) & ~((uintptr_t)(getpagesize() - 1))))
#define unprotect(addr) (mprotect(GET_PAGE(addr), getpagesize(), PROT_READ | PROT_WRITE | PROT_EXEC))
#define reprotect(addr) (mprotect(GET_PAGE(addr), getpagesize(), PROT_READ | PROT_EXEC))

int disassemble_x86(void *addr);
void print_disassemble_x86(void *addr, u_int32_t len);
void *detourFunction(void *orig, void *det);
void undetourFunction(void *orig, void *tramp);
int nopInstruction(void *addr);

// elf.c
Elf *initElf(const char *filename);
Elf_Scn *getSection(Elf *elf, char *sectionName);
GElf_Shdr *getSectionHeader(Elf *elf, char *sectionName);
GElf_Sym *getSymbol(const char *filename, const char *symbol);
int pltHook(const char *symbol, void *hook);

// utils.c
unsigned long crc32FromFile(const char *filename);

// vfunction.c
int getVTblIndex(const char *fmt, ...);

#define ADDRTYPE unsigned long
#define VTBL(classptr) (*(ADDRTYPE *)classptr)
#define PVFN_(classptr, offset) (VTBL(classptr) + offset)
#define VFN_(classptr, offset) *(ADDRTYPE *)PVFN_(classptr, offset)
#define PVFN(classptr, offset) PVFN_(classptr, (offset * sizeof(void *)))
#define VFN(classptr, offset) VFN_(classptr, (offset * sizeof(void *)))

#define DEFVFUNC( funcname , returntype , proto ) \
	typedef returntype (* funcname##Func ) proto ; \
	funcname##Func funcname = NULL; 
#define HOOKVFUNC( classptr , index , funcname , newfunc ) \
	mprotect(GET_PAGE(*(unsigned int **)classptr), 4096, PROT_READ | PROT_WRITE | PROT_EXEC); \
	funcname = ( funcname##Func )VFN( classptr , getVTblIndex("%p", index) ); \
	*(ADDRTYPE*)PVFN( classptr , getVTblIndex("%p", index) ) = (unsigned int)newfunc ; \
	mprotect(GET_PAGE(*(unsigned int **)classptr), 4096, PROT_READ | PROT_WRITE | PROT_EXEC);

#endif /*GHF_H_*/
