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

#ifndef TYPES_H_
#define TYPES_H_

typedef vmHeader_t *VM_LoadQVM_t(vm_t *vm, qboolean alloc);
typedef void VM_Free_t(vm_t *vm);
typedef int VM_CallCompiled_t(vm_t *vm, int *args);
typedef void *VM_ArgPtr_t(intptr_t intValue);

typedef void RE_RenderScene_t(refdef_t *fd);
typedef intptr_t CL_CgameSystemCalls_t(intptr_t *args);
typedef void CM_BoxTrace_t(trace_t *results, const vec3_t start, const vec3_t end, vec3_t mins, vec3_t maxs, clipHandle_t model, int brushmask, int capsule);
typedef int R_LerpTag_t(orientation_t *tag, qhandle_t handle, int startFrame, int endFrame, float frac, const char *tagName);
typedef void Cbuf_ExecuteText_t(int exec_when, const char *text);
typedef qboolean Key_IsDown_t(int keynum);
typedef model_t *R_GetModelByHandle_t(qhandle_t hModel);
typedef qhandle_t RE_RegisterShader_t(const char *name);
typedef void RE_AddRefEntityToScene_t(const refEntity_t *ent);
typedef shader_t *R_GetShaderByHandle_t(qhandle_t hShader);

typedef void CM_TransformedBoxTrace_t(trace_t *results, vec3_t start, vec3_t end, vec3_t mins, vec3_t maxs, clipHandle_t model, int brushmask, vec3_t origin, vec3_t angles, int capsule);
typedef clipHandle_t CM_TempBoxModel_t(const vec3_t mins, const vec3_t maxs, int capsule);
typedef clipHandle_t CM_InlineModel_t(int index);

typedef enum qvmType_t {
	UIGAME_QVM,
	CGAME_QVM
};

typedef struct urtQvm_t {
	char *version;
	qvmType_t type;
	u_int32_t crc32;

	// offsets
	uintptr_t cg;
	uintptr_t cg_entities;
	u_int centity_t_size;
};

typedef unsigned char byte;

#endif /*TYPES_H_*/
