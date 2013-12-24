
#ifndef LIBMODELING_WORLD_H
#define LIBMODELING_WORLD_H

#include <drag2d.h>

namespace libmodeling
{
	class World
	{
	public:
		d2d::Vector gravity;
		bool allowSleep;

		bool warmStarting;
		bool continuousPhysics;
		bool subStepping;

		int velocityIterations;
		int positionIterations;

	public:
		World() {
			gravity.set(0, -10);
			allowSleep = true;

			warmStarting = true;
			continuousPhysics = true;
			subStepping = false;

			velocityIterations = 8;
			positionIterations = 3;
		}

	}; // World
}

#endif // LIBMODELING_WORLD_H