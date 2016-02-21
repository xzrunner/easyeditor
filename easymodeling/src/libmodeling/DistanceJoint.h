#ifndef _EASYMODELING_DISTANCE_JOINT_H_
#define _EASYMODELING_DISTANCE_JOINT_H_

#include "Joint.h"

#include <ee/Vector.h>

namespace emodeling
{

class DistanceJoint : public Joint
{
public:
	DistanceJoint(Body* b0, Body* b1);

	virtual bool IsContain(const ee::Vector& pos) const;
	virtual bool IsIntersect(const ee::Rect& rect) const;

	virtual void Draw(DrawType type) const;

	ee::Vector GetWorldAnchorA() const;
	ee::Vector GetWorldAnchorB() const;

	void SetLocalAnchorA(const ee::Vector& world);
	void SetLocalAnchorB(const ee::Vector& world);

private:
	void DrawAnchor(const ee::Vector& pos, DrawType type) const;

	void DrawConnection(const ee::Vector& worldAnchorA, 
		const ee::Vector& worldAnchorB, DrawType type) const;

public:
	ee::Vector m_local_anchor_a;
	ee::Vector m_local_anchor_b;

	float m_frequency_hz;
	float m_damping_ratio;

}; // DistanceJoint

}

#endif // _EASYMODELING_DISTANCE_JOINT_H_
