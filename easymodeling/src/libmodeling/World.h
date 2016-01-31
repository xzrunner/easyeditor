
#ifndef LIBMODELING_WORLD_H
#define LIBMODELING_WORLD_H



namespace libmodeling
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

#endif // LIBMODELING_WORLD_H