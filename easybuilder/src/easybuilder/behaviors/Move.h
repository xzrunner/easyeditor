
#ifndef EBUILDER_MOVE_H
#define EBUILDER_MOVE_H

#include "Behavior.h"

namespace ebuilder
{
	class Move : public Behavior
	{
	private:
		Move(Actor* actor);

	public:
		ee::Vector start, end;

		float speed;

		friend class BehaviorFactory;

	}; // Move
}

#endif // EBUILDER_MOVE_H
