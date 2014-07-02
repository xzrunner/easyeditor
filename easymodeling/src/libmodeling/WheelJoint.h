
#ifndef LIBMODELING_WHEEL_JOINT_H
#define LIBMODELING_WHEEL_JOINT_H

#include "Joint.h"

namespace libmodeling
{
	class WheelJoint : public Joint
	{
	public:
		WheelJoint(Body* b0, Body* b1);

		virtual bool isContain(const d2d::Vector& pos) const;
		virtual bool isIntersect(const d2d::Rect& rect) const;

		virtual void draw(const d2d::Screen& scr, DrawType type) const;

		d2d::Vector getWorldAnchorA() const;
		d2d::Vector getWorldAnchorB() const;

		void setLocalAnchorA(const d2d::Vector& world);
		void setLocalAnchorB(const d2d::Vector& world);

	private:
		void drawAxisALine(const d2d::Screen& scr, const d2d::Vector& worldAnchorA) const;
		void drawFootBLine(const d2d::Screen& scr, const d2d::Vector& worldAnchorA, const d2d::Vector& worldAnchorB) const;

		void drawAnchorA(const d2d::Screen& scr, const d2d::Vector& pos, DrawType type) const;
		void drawAnchorB(const d2d::Screen& scr, const d2d::Vector& pos, DrawType type) const;

	public:
		d2d::Vector localAnchorA;
		d2d::Vector localAnchorB;

		d2d::Vector localAxisA;

		bool enableMotor;
		float maxMotorTorque;

		float motorSpeed;

		float frequencyHz;
		float dampingRatio;

	}; // WheelJoint
}

#endif // LIBMODELING_WHEEL_JOINT_H
