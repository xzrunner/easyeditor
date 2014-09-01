
#ifndef LIBMODELING_JOINT_H
#define LIBMODELING_JOINT_H

#include <drag2d.h>

namespace libmodeling
{
	class Body;

	class Joint : public d2d::Object
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
		
		virtual bool isContain(const d2d::Vector& pos) const = 0;
		virtual bool isIntersect(const d2d::Rect& rect) const = 0;

		virtual void draw(const d2d::Screen& scr, DrawType type) const = 0;

	protected:
		void drawBodyFlag(const d2d::Screen& scr) const;

		static d2d::Vector transWorldToLocal(const d2d::Vector& world, 
			const d2d::ISprite* sprite);
		static d2d::Vector transLocalToWorld(const d2d::Vector& local, 
			const d2d::ISprite* sprite);

	private:
		void drawBodyFlag(const d2d::Screen& scr, const d2d::Vector& pos) const;

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
