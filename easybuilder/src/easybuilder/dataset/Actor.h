
#ifndef EBUILDER_ACTOR_H
#define EBUILDER_ACTOR_H



namespace ebuilder
{
	class Behavior;

	class Actor : public ee::ImageSprite
	{
	public:
		Actor(const Actor& actor);
		Actor(ee::ImageSymbol* symbol);
		virtual ~Actor();

		//
		// ee::ICloneable interface
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
