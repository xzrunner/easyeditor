
#ifndef LIBMODELING_REVOLUTE_JOINT_H
#define LIBMODELING_REVOLUTE_JOINT_H

#include "Joint.h"

namespace libmodeling
{
	class RevoluteJoint : public Joint
	{
	public:
		RevoluteJoint(Body* b0, Body* b1);

		virtual bool isContain(const d2d::Vector& pos) const;
		virtual bool isIntersect(const d2d::Rect& rect) const;

		virtual void draw(const d2d::Screen& scr, DrawType type) const;

		d2d::Vector getWorldAnchorA() const;
		d2d::Vector getWorldAnchorB() const;

		void setLocalAnchorA(const d2d::Vector& world);
		void setLocalAnchorB(const d2d::Vector& world);

	private:
		void drawAnchor(const d2d::Screen& scr, const d2d::Vector& pos, DrawType type) const;

	public:
		d2d::Vector localAnchorA;
		d2d::Vector localAnchorB;

		float referenceAngle;

		bool enableLimit;
		float lowerAngle;
		float upperAngle;

		bool enableMotor;
		float maxMotorTorque;

		float motorSpeed;

	}; // RevoluteJoint
}

#endif // LIBMODELING_REVOLUTE_JOINT_H
