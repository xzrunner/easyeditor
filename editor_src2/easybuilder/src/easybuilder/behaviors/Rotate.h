
#ifndef EBUILDER_ROTATE_H
#define EBUILDER_ROTATE_H

#include "Behavior.h"

namespace ebuilder
{
	class Rotate : public Behavior
	{
	private:
		Rotate(Actor* actor);

	public:
		float start, end;

		float speed;

		friend class BehaviorFactory;

	}; // Rotate
}

#endif // EBUILDER_ROTATE_H
