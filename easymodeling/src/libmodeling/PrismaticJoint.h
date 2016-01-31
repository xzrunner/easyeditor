
#ifndef LIBMODELING_PRISMATIC_JOINT_H
#define LIBMODELING_PRISMATIC_JOINT_H

#include "Joint.h"

namespace libmodeling
{
	class PrismaticJoint : public Joint
	{
	public:
		PrismaticJoint(Body* b0, Body* b1);

		virtual bool isContain(const ee::Vector& pos) const;
		virtual bool isIntersect(const ee::Rect& rect) const;

		virtual void draw(DrawType type) const;

		ee::Vector getWorldAnchorA() const;
		ee::Vector getWorldAnchorB() const;

		void setLocalAnchorA(const ee::Vector& world);
		void setLocalAnchorB(const ee::Vector& world);

	private:
		void drawAnchor(const ee::Vector& pos, DrawType type) const;

	public:
		ee::Vector localAnchorA;
		ee::Vector localAnchorB;

		ee::Vector localAxisA;

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
