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

void urth_CG_ClipMoveToEntities(vec3_t start, vec3_t mins, vec3_t maxs, vec3_t end, int skipNumber, int mask, trace_t *tr) {
	int x, zd, zu;
	trace_t trace;
	entityState_t *ent;
	clipHandle_t cmodel;
	vec3_t bmins, bmaxs;
	vec3_t origin, angles;
	centity_t *cent;

	for (int i = 0; i < *cgame->cg_numSolidEntities; i++) {
		cent = (centity_t *)cgame->VM_ArgPtr((intptr_t)cgame->cg_solidEntities[i]);
		ent = &cent->currentState;

		if (ent->number == skipNumber)
			continue;

		if ( ent->solid == SOLID_BMODEL ) {
			// special value for bmodel
			cmodel = urt->CM_InlineModel(ent->modelindex);
			VectorCopy((float *)(cent + 0xa90 + sizeof(vec3_t)), angles);
			BG_EvaluateTrajectory(&cent->currentState.pos, cgame->nextSnap->serverTime, origin);
		} else {
			// encoded bbox
			x = (ent->solid & 255);
			zd = ((ent->solid>>8) & 255);
			zu = ((ent->solid>>16) & 255) - 32;

			bmins[0] = bmins[1] = -x;
			bmaxs[0] = bmaxs[1] = x;
			bmins[2] = -zd;
			bmaxs[2] = zu;

			cmodel = urt->CM_TempBoxModel(bmins, bmaxs, qfalse);
			VectorCopy(vec3_origin, angles);
			VectorCopy(cent->lerpOrigin, origin);
		}

		urt->CM_TransformedBoxTrace(&trace, start, end, mins, maxs, cmodel,  mask, origin, angles, qfalse);

		if (trace.allsolid || trace.fraction < tr->fraction) {
			trace.entityNum = ent->number;
			*tr = trace;
		} else if (trace.startsolid) {
			tr->startsolid = qtrue;
		}
		if (tr->allsolid) {
			return;
		}
	}
}

/*
================
CG_Trace
================
*/
void urth_CG_Trace(trace_t *result, vec3_t start, vec3_t mins, vec3_t maxs, vec3_t end, int skipNumber, int mask) {
	trace_t	t;

	urt->CM_BoxTrace(&t, start, end, mins, maxs, 0, mask, qfalse);
	t.entityNum = t.fraction != 1.0 ? ENTITYNUM_WORLD : ENTITYNUM_NONE;
	// check all other solid models
	urth_CG_ClipMoveToEntities(start, mins, maxs, end, skipNumber, mask, &t);

	*result = t;
}
