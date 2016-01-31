
#ifndef LIBMODELING_MOTOR_JOINT_H
#define LIBMODELING_MOTOR_JOINT_H

#include "Joint.h"

namespace libmodeling
{
	class MotorJoint : public Joint
	{
	public:
		MotorJoint(Body* b0, Body* b1);

		virtual bool isContain(const ee::Vector& pos) const;
		virtual bool isIntersect(const ee::Rect& rect) const;

		virtual void draw(DrawType type) const;

	private:
		void drawAnchor(const ee::Vector& pos, DrawType type) const;

	public:
		ee::Vector linearOffset;
		float angularOffset;

		float maxForce;

		float maxTorque;

		float correctionFactor;

	}; // MotorJoint
}

#endif // LIBMODELING_MOTOR_JOINT_H