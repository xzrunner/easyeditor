
#ifndef LIBMODELING_MOTOR_JOINT_H
#define LIBMODELING_MOTOR_JOINT_H

#include "Joint.h"

namespace libmodeling
{
	class MotorJoint : public Joint
	{
	public:
		MotorJoint(Body* b0, Body* b1);

		virtual bool isContain(const d2d::Vector& pos) const;
		virtual bool isIntersect(const d2d::Rect& rect) const;

		virtual void draw(DrawType type) const;

	private:
		void drawAnchor(const d2d::Vector& pos, DrawType type) const;

	public:
		d2d::Vector linearOffset;
		float angularOffset;

		float maxForce;

		float maxTorque;

		float correctionFactor;

	}; // MotorJoint
}

#endif // LIBMODELING_MOTOR_JOINT_H