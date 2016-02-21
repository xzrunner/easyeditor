#ifndef _EASYMODELING_WORLD_H_
#define _EASYMODELING_WORLD_H_

#include <ee/Vector.h>

namespace emodeling
{

class World
{
public:
	ee::Vector gravity;
	bool allowSleep;

	bool warmStarting;
	bool continuousPhysics;
	bool subStepping;

	int velocityIterations;
	int positionIterations;

public:
	World() {
		gravity.Set(0, -10);
		allowSleep = true;

		warmStarting = true;
		continuousPhysics = true;
		subStepping = false;

		velocityIterations = 8;
		positionIterations = 3;
	}

}; // World

}

#endif // _EASYMODELING_WORLD_H_