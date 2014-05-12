
#ifndef EBUILDER_MOUSE_BUTTON_H
#define EBUILDER_MOUSE_BUTTON_H

#include "Behavior.h"

namespace ebuilder
{
	class MouseButton : public Behavior
	{
	public:
		virtual ~MouseButton();

	private:
		MouseButton(Actor* actor);

	public:
		bool bPress;

		std::vector<Behavior*> satisfies;
		std::vector<Behavior*> notSatisfies;

		friend class BehaviorFactory;

	}; // MouseButton
}

#endif // EBUILDER_MOUSE_BUTTON_H
