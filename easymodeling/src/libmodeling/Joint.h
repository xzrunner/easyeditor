
#ifndef LIBMODELING_JOINT_H
#define LIBMODELING_JOINT_H



namespace libmodeling
{
	class Body;

	class Joint : public ee::Object
	{
	public:
		enum Type
		{
			e_revoluteJoint,
			e_prismaticJoint,
			e_distanceJoint,
			e_pulleyJoint,
			e_mouseJoint,
			e_gearJoint,
			e_wheelJoint,
			e_weldJoint,
			e_frictionJoint,
			e_ropeJoint,
			e_motorJoint
		};

		enum DrawType
		{
			e_default,
			e_mouseOn,
			e_selected
		};

	public:
		Joint(Body* b0, Body* b1, Type type);
		virtual ~Joint() {}
		
		virtual bool isContain(const ee::Vector& pos) const = 0;
		virtual bool isIntersect(const ee::Rect& rect) const = 0;

		virtual void draw(DrawType type) const = 0;

	protected:
		void drawBodyFlag() const;

		static ee::Vector transWorldToLocal(const ee::Vector& world, 
			const ee::Sprite* sprite);
		static ee::Vector transLocalToWorld(const ee::Vector& local, 
			const ee::Sprite* sprite);

	private:
		void drawBodyFlag(const ee::Vector& pos) const;

	protected:
		static const float JOINT_RADIUS_OUT;
		static const float JOINT_RADIUS_IN;
		static const float JOINT_RADIUS_SELECT;

	public:
		wxString m_name;

		Type type;

		Body* bodyA;
		Body* bodyB;

		bool collideConnected;

	}; // Joint
}

#endif // LIBMODELING_JOINT_H
