
#ifndef EBUILDER_PREVIEW_SCALE_SIZE_H
#define EBUILDER_PREVIEW_SCALE_SIZE_H

#include "Behavior.h"



namespace ebuilder
{	
	namespace preview
	{
		class ScaleSize : public Behavior
		{
		public:
			ScaleSize(ee::ISprite* sprite, float scale);

			virtual void trigger();

		private:
			ee::ISprite* m_sprite;

			float m_scale;

		}; // ScaleSize
	}
}

#endif // EBUILDER_PREVIEW_SCALE_SIZE_H
