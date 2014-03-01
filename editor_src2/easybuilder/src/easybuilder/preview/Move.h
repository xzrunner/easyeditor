
#ifndef EBUILDER_PREVIEW_MOVE_H
#define EBUILDER_PREVIEW_MOVE_H

#include "Behavior.h"
#include "AnimationControl.h"

#include <drag2d.h>

namespace ebuilder
{	
	namespace preview
	{
		class Move : public Behavior, public ITimeObserver
		{
		public:
			Move(d2d::ISprite* sprite, const d2d::Vector& start, 
				const d2d::Vector& end, float speed);

			virtual void trigger();

			virtual void onTick();

		private:
			d2d::ISprite* m_sprite;

			d2d::Vector m_start, m_end;

			float m_speed;

			wxMilliClock_t m_startTime;

		}; // Move
	}
}

#endif // EBUILDER_PREVIEW_MOVE_H
