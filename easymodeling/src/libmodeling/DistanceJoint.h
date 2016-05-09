#ifndef _EASYMODELING_DISTANCE_JOINT_H_
#define _EASYMODELING_DISTANCE_JOINT_H_

#include "Joint.h"

namespace emodeling
{

class DistanceJoint : public Joint
{
public:
	DistanceJoint(Body* b0, Body* b1);

	virtual bool IsContain(const sm::vec2& pos) const;
	virtual bool IsIntersect(const ee::Rect& rect) const;

	virtual void Draw(DrawType type) const;

	sm::vec2 GetWorldAnchorA() const;
	sm::vec2 GetWorldAnchorB() const;

	void SetLocalAnchorA(const sm::vec2& world);
	void SetLocalAnchorB(const sm::vec2& world);

private:
	void DrawAnchor(const sm::vec2& pos, DrawType type) const;

	void DrawConnection(const sm::vec2& worldAnchorA, 
		const sm::vec2& worldAnchorB, DrawType type) const;

public:
	sm::vec2 m_local_anchor_a;
	sm::vec2 m_local_anchor_b;

	float m_frequency_hz;
	float m_damping_ratio;

}; // DistanceJoint

}

#endif // _EASYMODELING_DISTANCE_JOINT_H_
