
#ifndef EBUILDER_PREVIEW_BUTTON_H
#define EBUILDER_PREVIEW_BUTTON_H

#include "Item.h"

namespace ebuilder
{	
	namespace preview
	{
		class Behavior;

		class Button : public Item
		{
		public:
			Button(const ee::ImageSprite& sprite);

		public:
			std::vector<Behavior*> m_pressBehaviors;
			std::vector<Behavior*> m_liftBehaviors;

		}; // Button
	}
}

#endif // EBUILDER_PREVIEW_BUTTON_H
