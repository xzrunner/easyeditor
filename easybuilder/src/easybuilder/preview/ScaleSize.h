
#ifndef EBUILDER_PREVIEW_SCALE_SIZE_H
#define EBUILDER_PREVIEW_SCALE_SIZE_H

#include "Behavior.h"

#include <drag2d.h>

namespace ebuilder
{	
	namespace preview
	{
		class ScaleSize : public Behavior
		{
		public:
			ScaleSize(d2d::ISprite* sprite, float scale);

			virtual void trigger();

		private:
			d2d::ISprite* m_sprite;

			float m_scale;

		}; // ScaleSize
	}
}

#endif // EBUILDER_PREVIEW_SCALE_SIZE_H
