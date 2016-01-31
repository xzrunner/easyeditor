
#ifndef LIBMODELING_GEAR_JOINT_H
#define LIBMODELING_GEAR_JOINT_H

#include "Joint.h"

namespace libmodeling
{
	class GearJoint : public Joint
	{
	public:
		GearJoint(Body* b0, Body* b1, Joint* j1, Joint* j2);

		virtual bool isContain(const ee::Vector& pos) const;
		virtual bool isIntersect(const ee::Rect& rect) const;

		virtual void draw(DrawType type) const;

	public:
		Joint* joint1;
		Joint* joint2;

		float ratio;

	}; // GearJoint
}

#endif // LIBMODELING_GEAR_JOINT_H
