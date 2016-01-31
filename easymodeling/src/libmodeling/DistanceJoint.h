
#ifndef LIBMODELING_DISTANCE_JOINT_H
#define LIBMODELING_DISTANCE_JOINT_H

#include "Joint.h"

namespace libmodeling
{
	class DistanceJoint : public Joint
	{
	public:
		DistanceJoint(Body* b0, Body* b1);

		virtual bool isContain(const ee::Vector& pos) const;
		virtual bool isIntersect(const ee::Rect& rect) const;

		virtual void draw(DrawType type) const;

		ee::Vector getWorldAnchorA() const;
		ee::Vector getWorldAnchorB() const;

		void setLocalAnchorA(const ee::Vector& world);
		void setLocalAnchorB(const ee::Vector& world);

	private:
		void drawAnchor(const ee::Vector& pos, DrawType type) const;

		void drawConnection(const ee::Vector& worldAnchorA, 
			const ee::Vector& worldAnchorB, DrawType type) const;

	public:
		ee::Vector localAnchorA;
		ee::Vector localAnchorB;

		float frequencyHz;
		float dampingRatio;

	}; // DistanceJoint
}

#endif // LIBMODELING_DISTANCE_JOINT_H
