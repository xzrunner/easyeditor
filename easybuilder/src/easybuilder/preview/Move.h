
#ifndef EBUILDER_PREVIEW_MOVE_H
#define EBUILDER_PREVIEW_MOVE_H

#include "Behavior.h"
#include "AnimationControl.h"



namespace ebuilder
{	
	namespace preview
	{
		class Move : public Behavior, public ITimeObserver
		{
		public:
			Move(ee::ISprite* sprite, const ee::Vector& start, 
				const ee::Vector& end, float speed);

			virtual void trigger();

			virtual void onTick();

		private:
			ee::ISprite* m_sprite;

			ee::Vector m_start, m_end;

			float m_speed;

			wxMilliClock_t m_startTime;

		}; // Move
	}
}

#endif // EBUILDER_PREVIEW_MOVE_H
