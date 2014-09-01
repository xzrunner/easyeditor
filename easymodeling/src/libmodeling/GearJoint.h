
#ifndef LIBMODELING_GEAR_JOINT_H
#define LIBMODELING_GEAR_JOINT_H

#include "Joint.h"

namespace libmodeling
{
	class GearJoint : public Joint
	{
	public:
		GearJoint(Body* b0, Body* b1, Joint* j1, Joint* j2);

		virtual bool isContain(const d2d::Vector& pos) const;
		virtual bool isIntersect(const d2d::Rect& rect) const;

		virtual void draw(const d2d::Screen& scr, DrawType type) const;

	public:
		Joint* joint1;
		Joint* joint2;

		float ratio;

	}; // GearJoint
}

#endif // LIBMODELING_GEAR_JOINT_H
