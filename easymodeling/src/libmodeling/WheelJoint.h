
#ifndef LIBMODELING_WHEEL_JOINT_H
#define LIBMODELING_WHEEL_JOINT_H

#include "Joint.h"

namespace libmodeling
{
	class WheelJoint : public Joint
	{
	public:
		WheelJoint(Body* b0, Body* b1);

		virtual bool isContain(const ee::Vector& pos) const;
		virtual bool isIntersect(const ee::Rect& rect) const;

		virtual void draw(DrawType type) const;

		ee::Vector getWorldAnchorA() const;
		ee::Vector getWorldAnchorB() const;

		void setLocalAnchorA(const ee::Vector& world);
		void setLocalAnchorB(const ee::Vector& world);

	private:
		void drawAxisALine(const ee::Vector& worldAnchorA) const;
		void drawFootBLine(const ee::Vector& worldAnchorA, const ee::Vector& worldAnchorB) const;

		void drawAnchorA(const ee::Vector& pos, DrawType type) const;
		void drawAnchorB(const ee::Vector& pos, DrawType type) const;

	public:
		ee::Vector localAnchorA;
		ee::Vector localAnchorB;

		ee::Vector localAxisA;

		bool enableMotor;
		float maxMotorTorque;

		float motorSpeed;

		float frequencyHz;
		float dampingRatio;

	}; // WheelJoint
}

#endif // LIBMODELING_WHEEL_JOINT_H
