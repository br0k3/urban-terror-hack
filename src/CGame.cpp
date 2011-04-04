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

CGame::CGame(vm_t *vm) : Qvm(vm)  {
	// Init all pointers

	for (int entityNum = 0; entityNum < MAX_GENTITIES; entityNum++)
		this->cg_entities[entityNum] = (centity_t *)((uintptr_t)this->VM_ArgPtr(this->qvm->cg_entities) + (entityNum * this->qvm->centity_t_size));

	for (int entityNum = 0; entityNum < MAX_CLIENTS; entityNum++)
		this->clientInfo[entityNum] = (clientInfo_t *)((uintptr_t)this->VM_ArgPtr(0x34a34c) + (entityNum * 0x9b8));

	this->cg = (cg_t *)this->VM_ArgPtr(this->qvm->cg);
	
	this->blueShader = 0;
	this->redShader = 0;
	this->smokeShader = 0;
	this->viewSmokeShader = 0;
	this->scopedShader = 0;
	
	this->snap = NULL;
	this->nextSnap = NULL;
	
	this->lastNameChange = 0;
	
	this->cg_numSolidEntities = (int *)this->VM_ArgPtr(0x38d28c); // old 0x38d274
	this->cg_solidEntities = (centity_t **)this->VM_ArgPtr(0x38ce8c); // old offset - 0x38ce74
}

CGame::~CGame() {
}

bool CGame::isVisible(vec3_t target) {
	trace_t trace;
	urth_CG_Trace(&trace, this->refdef.vieworg, vec3_origin, vec3_origin, target, 0, MASK_SHOT);
	return (trace.fraction == 1.0f);
}

float *CGame::getLerpOrigin(int entityNum) {
	return (float *)((byte *)this->cg_entities[entityNum] + 0xa90);
}

void CGame::doAimbot() {
	int nearest = -1;
	float distance = -1;

	for (int clientNum=0; clientNum<MAX_CLIENTS; clientNum++) {
		// Valid
		if (!this->cg_entities[clientNum]->currentValid)
			continue;

		// Myself
		if (clientNum == this->cg->clientNum)
			continue;

		// Dead
		if (this->cg_entities[clientNum]->currentState.eFlags & EF_DEAD)
			continue;

		// Enemy
		if (this->clientInfo[clientNum]->team == this->clientInfo[this->cg->clientNum]->team)
			continue;

		// Visibility
		if (!this->isVisible(this->playersHead[clientNum]))
			continue;
		
		float dist = Distance(this->playersHead[clientNum], this->refdef.vieworg);
		if ((nearest == -1) || (dist < distance)) {
			nearest = clientNum;
			distance = dist;
		}
	}
	
	bool shoot = urt->Key_IsDown(103) && (nearest != -1); // IF (h) KEY PRESSED

	if (shoot){	
		urt->aimAt(this->playersHead[nearest]);
	}

	this->doAutoShoot(shoot);
}

void CGame::doAutoShoot(bool shoot) {
	static bool shootState = false;
	if (shoot && !shootState) {
		urt->Cbuf_ExecuteText(EXEC_NOW, "+attack\n");
		shootState = true;
	} else if (!shoot && shootState) {
		urt->Cbuf_ExecuteText(EXEC_NOW, "-attack\n");
		shootState = false;
	}
}


/*void CGame::doNameStealing() {
	static int nameChangeTime = 30000;

	if (!this->lastNameChange)
		this->lastNameChange = cgame->snap->serverTime;

	if (this->lastNameChange + nameChangeTime > cgame->snap->serverTime)
		return;
		
	// Store all available team names
	std::vector<char *> names;

	// Get all available team names
	for (int clientNum=0; clientNum<MAX_CLIENTS; clientNum++) {
		clientInfo_t *ci = cgame->clientInfo[clientNum];
		if (!ci->infoValid)
			continue;
		
		// Skip myself
		if (clientNum == cgame->cg->clientNum)
			continue;
		
		// Skip other team
		if (ci->team != cgame->clientInfo[cgame->cg->clientNum]->team)
			continue;

		names.push_back(ci->name);
	}
	
	if (!names.size())
		return;
	
	// Choose a random name
	int choice = (int)((float)names.size() * (rand() / (RAND_MAX + 1.0))); 
	
	// Change name
	std::string cmd = "name \"";
	cmd += names[choice];
	cmd += " \"";
	urt->Cbuf_ExecuteText(EXEC_APPEND, cmd.c_str());
	
	this->lastNameChange = cgame->snap->serverTime;
}*/
