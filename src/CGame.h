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

#ifndef CGAME_H_
#define CGAME_H_

class CGame : public Qvm {
public:
	CGame(vm_t *vm);
	virtual ~CGame();
	
	// Pointers to original vars/functions
	centity_t *cg_entities[MAX_GENTITIES];
	clientInfo_t *clientInfo[MAX_CLIENTS];
	cg_t *cg;

	// For CG_Trace()
	int *cg_numSolidEntities;
	centity_t **cg_solidEntities;
	
	// Store head position for the aimbot
	vec3_t playersHead[MAX_CLIENTS];

	refdef_t refdef;
	snapshot_t *snap;
	snapshot_t *nextSnap;
	
	// Shaders
	qhandle_t blueShader;
	qhandle_t redShader;
	qhandle_t smokeShader;
	qhandle_t viewSmokeShader;
	qhandle_t scopedShader;
	
	int lastNameChange;
	
	bool isVisible(vec3_t target);
	float *getLerpOrigin(int entityNum);
	
	void doAimbot();
	void doAutoShoot(bool shoot);
	// void doNameStealing();
};

#endif /*CGAME_H_*/
