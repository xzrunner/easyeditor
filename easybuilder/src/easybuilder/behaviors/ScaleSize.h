
#ifndef EBUILDER_SCALE_SIZE_H
#define EBUILDER_SCALE_SIZE_H

#include "Behavior.h"

namespace ebuilder
{
	class ScaleSize : public Behavior
	{
	private:
		ScaleSize(Actor* actor);
		
	public:
		float val;

		friend class BehaviorFactory;

	}; // ScaleSize
}

#endif // EBUILDER_SCALE_SIZE_H
