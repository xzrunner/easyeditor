
#ifndef EBUILDER_PREVIEW_ROTATE_H
#define EBUILDER_PREVIEW_ROTATE_H

#include "Behavior.h"
#include "AnimationControl.h"

#include <drag2d.h>

namespace ebuilder
{
	namespace preview
	{
		class Rotate : public Behavior, public ITimeObserver
		{
		public:
			Rotate(d2d::ISprite* sprite, float start, 
				float end, float speed);

			virtual void trigger();

			virtual void onTick();

		private:
			d2d::ISprite* m_sprite;

			float m_start, m_end;

			float m_speed;

			wxMilliClock_t m_startTime;

		}; // Rotate
	}
}

#endif // EBUILDER_PREVIEW_ROTATE_H
