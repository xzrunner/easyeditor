
#ifndef EBUILDER_PREVIEW_ACTOR_H
#define EBUILDER_PREVIEW_ACTOR_H

#include "Item.h"

namespace ebuilder
{	
	namespace preview
	{
		class Actor : public Item
		{
		public:
			Actor(const ee::ImageSprite& sprite);

		}; // Actor
	}
}

#endif // EBUILDER_PREVIEW_ACTOR_H
