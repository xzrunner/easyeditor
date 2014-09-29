
#ifndef LIBMODELING_PRISMATIC_JOINT_H
#define LIBMODELING_PRISMATIC_JOINT_H

#include "Joint.h"

namespace libmodeling
{
	class PrismaticJoint : public Joint
	{
	public:
		PrismaticJoint(Body* b0, Body* b1);

		virtual bool isContain(const d2d::Vector& pos) const;
		virtual bool isIntersect(const d2d::Rect& rect) const;

		virtual void draw(DrawType type) const;

		d2d::Vector getWorldAnchorA() const;
		d2d::Vector getWorldAnchorB() const;

		void setLocalAnchorA(const d2d::Vector& world);
		void setLocalAnchorB(const d2d::Vector& world);

	private:
		void drawAnchor(const d2d::Vector& pos, DrawType type) const;

	public:
		d2d::Vector localAnchorA;
		d2d::Vector localAnchorB;

		d2d::Vector localAxisA;

		float referenceAngle;

		bool enableLimit;
		float lowerTranslation;
		float upperTranslation;

		bool enableMotor;
		float maxMotorForce;

		float motorSpeed;

	}; // PrismaticJoint
}

#endif // LIBMODELING_PRISMATIC_JOINT_H
