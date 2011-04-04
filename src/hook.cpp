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

vmHeader_t *urth_VM_LoadQVM(vm_t *vm, qboolean alloc) {
	unprotect(urt->VM_LoadQVM);		// FIXME: Why detour's tramp is reprotected ?
	vmHeader_t *result = urt->VM_LoadQVM(vm, alloc);

	// LOG_DEBUG("VM_LoadQVM(): " << vm->name << " vm " << std::hex << (void *)vm);

	if (!strcmp(vm->name, CGAME_QVM_NAME))
		cgame = new CGame(vm);
	else if (!strcmp(vm->name, UIGAME_QVM_NAME))
		uigame = new UIGame(vm);

	return result;
}

void urth_VM_Free(vm_t *vm) {
	if (!strcmp(vm->name, CGAME_QVM_NAME)) {
		delete(cgame);
		cgame = NULL;
	} else if (!strcmp(vm->name, UIGAME_QVM_NAME)) {
		delete(uigame);
		uigame = NULL;
	}

	urt->VM_Free(vm);
}

int urth_VM_CallCompiled(vm_t *vm, int *args) {
	int result = urt->VM_CallCompiled(vm, args);

	if (cgame && (vm == cgame->vm) && (args[0] == CG_INIT)) {
		// LOG_DEBUG("VM_CallCompiled(): cgame post CG_INIT");
		cgame->blueShader = urt->RE_RegisterShader("models/mapobjects/crystals/crystals");
		cgame->redShader = urt->RE_RegisterShader("models/mapobjects/crystals/crystals_red");
		cgame->smokeShader = urt->RE_RegisterShader("gfx/misc/grenadesmokepuff");
		cgame->viewSmokeShader = urt->RE_RegisterShader("gfx/misc/viewsmokepuff");
		cgame->scopedShader = urt->RE_RegisterShader("gfx/nvg/scope2");
	}

	return result; 
}

void urth_RE_RenderScene(refdef_t *fd) {
	// Remove sniper zoom
	// fd->fov_x = 90;
	// fd->fov_y = 73.7398;

	urt->RE_RenderScene(fd);
	
	// Sanity check
	if (!cgame)
		return;
	
	// Copy refdef
	cgame->refdef = *fd;

	if(urt->Key_IsDown(K_KP_NUMLOCK)){ // if num lock pressed activate aimbot
		cgame->doAimbot();		
	}

	// cgame->doNameStealing();
	
	// printf("cg_numSolidEntities: %i\n", *cgame->cg_numSolidEntities);
	// printf("%p\n", ((centity_t *)cgame->VM_ArgPtr((intptr_t)cgame->cg_solidEntities[0]))->currentState.number);
}

intptr_t urth_CL_CgameSystemCalls(intptr_t *args) {
	bool callSyscall = true;

	switch (args[0]) {
		case CG_R_ADDREFENTITYTOSCENE: {

			refEntity_t *refEnt = (refEntity_t *)cgame->VM_ArgPtr(args[1]);

			// Transparent smoke
			if ((refEnt->reType == RT_SPRITE) && (refEnt->customShader == cgame->smokeShader)) {
				refEnt->shaderRGBA[3] = 0x20;
				break;
			}

			// Only need plain model
			if (refEnt->reType != RT_MODEL)
				break;

			char *modelName = urt->R_GetModelByHandle(refEnt->hModel)->name;

			// Find the centity_t for this refEntity_t
			for (int clientNum=0; clientNum<MAX_CLIENTS; clientNum++) {
				// If valid player
				if (!cgame->cg_entities[clientNum]->currentValid)
					continue;
				
				// If same entity
				if (!VectorCompare(refEnt->lightingOrigin, cgame->getLerpOrigin(clientNum)))
					continue;
				
				// Add wallhack
				if(urt->Key_IsDown(K_KP_NUMLOCK)){ // if num lock pressed activate aimbot				
					refEnt->renderfx |= RF_DEPTHHACK;
				}
	
				// Check team
				if (cgame->clientInfo[cgame->cg->clientNum]->team == cgame->clientInfo[clientNum]->team)
					continue;

				// If head, backup origin for aimbot
				if (strstr(modelName, "head")) {
					vec3_t head;
					VectorCopy(refEnt->origin, head);

					// Add auto vecz correction based on distance
					static float veczMax = 5.0;
					static float veczMin = -3.0;
					static float distMin = 500;
					static float distMax = 2000;
					float dist = Distance(cgame->refdef.vieworg, head);
					
					float vecz = veczMin;
					if (dist < distMin)
						vecz = veczMax;
					else if (dist < distMax)
						vecz = ((dist - distMin) / (distMax - distMin)) * (veczMax - veczMin); 

					// Apply vecz
					head[ROLL] += vecz;
					
					// Add ping prediction
					VectorMA(
						head,
						cgame->nextSnap->ping / 1000 * 0.6,
						cgame->cg_entities[clientNum]->currentState.pos.trDelta,
						head
					);

					VectorCopy(head, cgame->playersHead[clientNum]);
				}

				// Add quad shader
				if(urt->Key_IsDown(K_KP_NUMLOCK)){ // if num lock pressed activate aimbot				
					if (cgame->isVisible(refEnt->origin))
						refEnt->customShader = cgame->redShader;
					else
						refEnt->customShader = cgame->blueShader;
				}
				urt->CL_CgameSystemCalls(args);
			}
			break;
		}
		case CG_R_DRAWSTRETCHPIC:
			// Check for smoked/scoped view
			if ((args[9] == cgame->viewSmokeShader) || (args[9] == cgame->scopedShader))
				callSyscall = false;
			break;
		case CG_GETSNAPSHOT:
			cgame->snap = cgame->snap;
			cgame->nextSnap = (snapshot_t *)cgame->VM_ArgPtr(args[2]);
			
			if (!cgame->snap)
				cgame->snap = cgame->nextSnap;
			break;
		default:
			break;
	}

	if (!callSyscall)
		return 0;

	intptr_t result = urt->CL_CgameSystemCalls(args);

	return result;
}
