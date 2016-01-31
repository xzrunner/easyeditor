
#ifndef LIBMODELING_WELD_JOINT_H
#define LIBMODELING_WELD_JOINT_H

#include "Joint.h"

namespace libmodeling
{
	class WeldJoint : public Joint
	{
	public:
		WeldJoint(Body* b0, Body* b1);

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

		float referenceAngle;

		float frequencyHz;
		float dampingRatio;

	}; // WeldJoint
}

#endif // LIBMODELING_WELD_JOINT_H