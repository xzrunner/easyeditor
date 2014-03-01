
#ifndef EBUILDER_ACTOR_H
#define EBUILDER_ACTOR_H

#include <drag2d.h>

namespace ebuilder
{
	class Behavior;

	class Actor : public d2d::ImageSprite
	{
	public:
		Actor(const Actor& actor);
		Actor(d2d::ImageSymbol* symbol);
		virtual ~Actor();

		//
		// d2d::ICloneable interface
		//
		virtual Actor* clone();

		const std::vector<Behavior*>& getBehaviors() const {
			return m_behaviors;
		}
		void addBehavior(Behavior* behavior) {
			m_behaviors.push_back(behavior);
		}
		void clearBehaviors();

	public:
		bool m_isBg;

	private:
		std::vector<Behavior*> m_behaviors;

	}; // Actor
}

#endif // EBUILDER_ACTOR_H
