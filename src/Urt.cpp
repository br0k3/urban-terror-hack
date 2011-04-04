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

Urt::Urt() {
	this->initHook();
}

Urt::~Urt() {
	// TODO: Not need but for clean code, unhook everything here
}

void Urt::initHook() {
	// Qvm
	DO_HOOK(VM_LoadQVM, "VM_LoadQVM");
	DO_HOOK(VM_Free, "VM_Free");
	DO_HOOK(VM_CallCompiled, "VM_CallCompiled");

	// Render
	DO_HOOK(RE_RenderScene, "RE_RenderScene");
	this->CM_BoxTrace = (CM_BoxTrace_t *)getSymbol(NULL, "CM_BoxTrace")->st_value;
	this->R_LerpTag = (R_LerpTag_t *)getSymbol(NULL, "R_LerpTag")->st_value;
	this->R_GetModelByHandle = (R_GetModelByHandle_t *)getSymbol(NULL, "R_GetModelByHandle")->st_value;
	this->RE_RegisterShader = (RE_RegisterShader_t *)getSymbol(NULL, "RE_RegisterShader")->st_value;
	this->RE_AddRefEntityToScene = (RE_AddRefEntityToScene_t *)getSymbol(NULL, "RE_AddRefEntityToScene")->st_value;
	this->R_GetShaderByHandle = (R_GetShaderByHandle_t *)getSymbol(NULL, "R_GetShaderByHandle")->st_value;

	this->CM_TransformedBoxTrace = (CM_TransformedBoxTrace_t *)getSymbol(NULL, "CM_TransformedBoxTrace")->st_value;
	this->CM_TempBoxModel = (CM_TempBoxModel_t *)getSymbol(NULL, "CM_TempBoxModel")->st_value;
	this->CM_InlineModel = (CM_InlineModel_t *)getSymbol(NULL, "CM_InlineModel")->st_value;

	// Client
	this->Cbuf_ExecuteText = (Cbuf_ExecuteText_t *)getSymbol(NULL, "Cbuf_ExecuteText")->st_value;
	this->Key_IsDown = (Key_IsDown_t *)getSymbol(NULL, "Key_IsDown")->st_value;
	DO_HOOK(CL_CgameSystemCalls, "CL_CgameSystemCalls");
	this->cl = (clientActive_t *)getSymbol(NULL, "cl")->st_value;
}

void Urt::aimAt(float x, float y) {
	vec3_t refdefViewAngles;
	vectoangles(cgame->refdef.viewaxis[0], refdefViewAngles);
	
	this->cl->viewangles[PITCH] -= AngleDelta(x, refdefViewAngles[PITCH]);
	this->cl->viewangles[YAW] -= AngleDelta(y, refdefViewAngles[YAW]);
}

void Urt::aimAt(vec3_t target) {
	vec3_t refdefViewAngles;
	vectoangles(cgame->refdef.viewaxis[0], refdefViewAngles);

	vec3_t org, ang;
	VectorSubtract(target, cgame->refdef.vieworg, org);
	vectoangles(org, ang);
	
	ang[PITCH] -= refdefViewAngles[PITCH];
	ang[YAW] -= refdefViewAngles[YAW];

	this->cl->viewangles[PITCH] += ang[PITCH];
	this->cl->viewangles[YAW] += ang[YAW];
}
