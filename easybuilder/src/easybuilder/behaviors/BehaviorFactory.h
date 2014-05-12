
#ifndef EBUILDER_BEHAVIOR_FACTORY_H
#define EBUILDER_BEHAVIOR_FACTORY_H

#include "BehaviorDefs.h"

namespace ebuilder
{
	class Behavior;
	class BehaviorPanel;
	class Actor;

	class BehaviorFactory
	{
	public:
		static Behavior* createBehavior(BehaviorType type, Actor* actor);

		static BehaviorPanel* createBehaviorPanel(wxWindow* parent, 
			Behavior* behavior);

	}; // BehaviorFactory
}

#endif // EBUILDER_BEHAVIOR_FACTORY_H
