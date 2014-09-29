
#ifndef LIBMODELING_DISTANCE_JOINT_H
#define LIBMODELING_DISTANCE_JOINT_H

#include "Joint.h"

namespace libmodeling
{
	class DistanceJoint : public Joint
	{
	public:
		DistanceJoint(Body* b0, Body* b1);

		virtual bool isContain(const d2d::Vector& pos) const;
		virtual bool isIntersect(const d2d::Rect& rect) const;

		virtual void draw(DrawType type) const;

		d2d::Vector getWorldAnchorA() const;
		d2d::Vector getWorldAnchorB() const;

		void setLocalAnchorA(const d2d::Vector& world);
		void setLocalAnchorB(const d2d::Vector& world);

	private:
		void drawAnchor(const d2d::Vector& pos, DrawType type) const;

		void drawConnection(const d2d::Vector& worldAnchorA, 
			const d2d::Vector& worldAnchorB, DrawType type) const;

	public:
		d2d::Vector localAnchorA;
		d2d::Vector localAnchorB;

		float frequencyHz;
		float dampingRatio;

	}; // DistanceJoint
}

#endif // LIBMODELING_DISTANCE_JOINT_H
