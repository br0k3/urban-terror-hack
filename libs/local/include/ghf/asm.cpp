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

#include <dis-asm.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

#include "ghf.h"

#define ASM_JUMP 0xe9
#define ASM_CALL 0xe8
#define ASM_NOP 0x90
#define ASM_MOV_VALUE_TO_EBX 0xbb
// mov    (%esp),%ebx;	ret
#define ASM_GET_PC_THUNK "\x8b\x1c\x24\xc3" 
#define ASM_JUMP_SIZE 5
#define ASM_CALL_SIZE 5

// Disassembler callbacks
int my_buffer_read_memory(bfd_vma from, bfd_byte *to, unsigned int length, struct disassemble_info *info) {
	memcpy((void *)to, (void *)(uintptr_t)from, length);
	return 0;
}

void my_perror_memory(int status, bfd_vma memaddr, struct disassemble_info *info) {
	info->fprintf_func(info->stream, "ghf: Unknown error %d\n", status);
	exit(EXIT_FAILURE);
}

// For disable disassembler output
fprintf_ftype my_fprintf(FILE *stream, const char *format, ...) {
	return 0;
}

// Return the length of the instruction
int disassemble_x86(void *addr) {
	// Initialize info for disassembler
	disassemble_info info;
	init_disassemble_info(&info, stdout, (fprintf_ftype)my_fprintf);
	info.mach = bfd_mach_i386_i386;
	info.read_memory_func = my_buffer_read_memory;
	info.memory_error_func = my_perror_memory;

	// Disassemble instruction and return length
	return print_insn_i386((bfd_vma)(uintptr_t)addr, &info);
}

// Use for debug
void print_disassemble_x86(void *addr, u_int32_t len) {
	// Initialize info for disassembler
	disassemble_info info;
	init_disassemble_info(&info, stdout, (fprintf_ftype)fprintf);
	info.mach = bfd_mach_i386_i386;
	info.read_memory_func = my_buffer_read_memory;
	info.memory_error_func = my_perror_memory;

	// Disassemble instructions
	u_int32_t count = 0;
	while (count < len) {
		printf("%p: ", addr + count);
		count += print_insn_i386((bfd_vma)(uintptr_t)addr + count, &info);
		printf("\n");
	}
	return;
}

// Return NULL if error and 'errno' is set
void *detourFunction(void *orig, void *det) {
	// Sanity check
	if (!orig || !det) {
		printf("detourFunction: error: bad args, orig: %p tramp: %p\n", orig, det);
		errno = EFAULT;
		return NULL;
	}

	// Get instructions length
	int iLen = 0;
	while (iLen < ASM_JUMP_SIZE)
		iLen += disassemble_x86(orig + iLen);

	// Backup instructions before it's override by the jump
	void *tramp = malloc(iLen + ASM_JUMP_SIZE);
	// Sanity check
	if (!tramp) {
		perror("detourFunction: error: malloc() tramp failed\n");
		errno = ENOMEM;
		return NULL;
	}
	int iCount = 0;
	while (iCount < iLen) {
		int len = disassemble_x86(orig + iCount);

		// Sanity check
		if (len < 1) {
			printf("error: can't disassemble at %p\n", orig);
			errno = EINVAL;
			return NULL;
		}

		// Search for a call in the backuped instructions and correct his value (because call are relative)
		if ((len == ASM_CALL_SIZE) && (*(unsigned char *)(orig + iCount) == ASM_CALL)) {
			// Check if the call is "__i686.get_pc_thunk.bx", if yes replace it by mov $offset, %ebx
			if (!memcmp((void *)((orig + iCount + 5) + *(uintptr_t *)(orig + iCount + 1)), ASM_GET_PC_THUNK, sizeof(ASM_GET_PC_THUNK) - 1)) {
				*(unsigned char *)(tramp + iCount) = ASM_MOV_VALUE_TO_EBX;
				*(void **)(tramp + iCount + 1) = (void *)(orig + iCount + 5);
			} else {
				*(unsigned char *)(tramp + iCount) = ASM_CALL;
				*(uintptr_t **)((uintptr_t)tramp + iCount + 1) = *(uintptr_t **)((uintptr_t)orig + iCount + 1) - (((uintptr_t)tramp + iCount + 1) - ((uintptr_t)orig + iCount + 1));
			}
		// If not a call, simply copy the instruction
		} else {
			memcpy(tramp + iCount, orig + iCount, len);
		}
		iCount += len;
	}
	
	// Write a jump to the original function after the backuped intructions
	*(unsigned char *)(tramp + iLen) = ASM_JUMP;
	*(void **)(tramp + iLen + 1) = (void *)((((uintptr_t)orig) + iLen) - (uintptr_t)(tramp + iLen + ASM_JUMP_SIZE));

	// Write a jump to the detour at the original function
	unprotect(orig);
	*(unsigned char *)orig = ASM_JUMP;
	*(void **)((uintptr_t)orig + 1) = (void *)(((uintptr_t)det) - (((uintptr_t)orig) + ASM_JUMP_SIZE));
	reprotect(orig);

	return tramp;
}

void undetourFunction(void *orig, void *tramp) {
	// Sanity check
	if (!orig || !tramp) {
		printf("undetourFunction: error: bad args, orig: %p tramp: %p\n", orig, tramp);
		raise(SIGSEGV);
	}
	
	unprotect(orig);

	int iCount = 0;
	while (iCount < ASM_JUMP_SIZE) {
		int len = disassemble_x86(tramp + iCount);

		// Search for a call in the backuped instructions and correct his value (because call are relative)
		if ((len == ASM_CALL_SIZE) && (*(unsigned char *)(tramp + iCount) == ASM_CALL)) {
			*(unsigned char *)(orig + iCount) = ASM_CALL;
			*(void **)((uintptr_t)orig + iCount + 1) = *(void **)((uintptr_t)tramp + iCount + 1) - (((uintptr_t)orig + iCount + 1) - ((uintptr_t)tramp + iCount + 1));
		// If not a call, simply copy the instruction
		} else {
			memcpy(orig + iCount, tramp + iCount, len);
		}
		iCount += len;
	}

	reprotect(orig);

	free(tramp);
}

int nopInstruction(void *addr) {
	unsigned int size = disassemble_x86(addr);

	unprotect(addr);
	int count = 0;
	for (; count < size; count++)
		*(unsigned char *)(addr + count) = ASM_NOP;
	reprotect(addr);

	return size;
}
