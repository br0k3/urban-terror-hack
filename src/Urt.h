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

#ifndef URT_H_
#define URT_H_

class Urt {
public:
	Urt();
	virtual ~Urt();

	void aimAt(float x, float y);
	void aimAt(vec3_t target);

	// Qvm original functions
	VM_LoadQVM_t *VM_LoadQVM;
	VM_Free_t *VM_Free;
	VM_CallCompiled_t *VM_CallCompiled;
	
	// Misc original functions/vars
	RE_RenderScene_t *RE_RenderScene;
	CL_CgameSystemCalls_t *CL_CgameSystemCalls;
	CM_BoxTrace_t *CM_BoxTrace;
	R_LerpTag_t *R_LerpTag;
	Cbuf_ExecuteText_t *Cbuf_ExecuteText;
	Key_IsDown_t *Key_IsDown;
	R_GetModelByHandle_t *R_GetModelByHandle;
	RE_RegisterShader_t *RE_RegisterShader;
	RE_AddRefEntityToScene_t *RE_AddRefEntityToScene;
	R_GetShaderByHandle_t *R_GetShaderByHandle;
	
	CM_TransformedBoxTrace_t *CM_TransformedBoxTrace;
	CM_TempBoxModel_t *CM_TempBoxModel;
	CM_InlineModel_t *CM_InlineModel;

	clientActive_t *cl;
	
private:
	void initHook();
};

// Hook helper
#define DO_HOOK(function, symbol)						\
	/* TODO: Check if the symbol is found */			\
	this->function =									\
		(function##_t *)detourFunction(					\
			(void *)getSymbol(NULL, symbol)->st_value,	\
			(void *)urth_##function						\
		)

#endif /*URT_H_*/
