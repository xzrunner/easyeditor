
#ifndef LIBMODELING_ROPE_JOINT_H
#define LIBMODELING_ROPE_JOINT_H

#include "Joint.h"

namespace libmodeling
{
	class RopeJoint : public Joint
	{
	public:
		RopeJoint(Body* b0, Body* b1);

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

		float maxLength;

	}; // RopeJoint
}

#endif // LIBMODELING_ROPE_JOINT_H