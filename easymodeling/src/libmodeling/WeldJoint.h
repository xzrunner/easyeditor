#ifndef _EASYMODELING_WELD_JOINT_H_
#define _EASYMODELING_WELD_JOINT_H_

#include "Joint.h"

namespace emodeling
{

class WeldJoint : public Joint
{
public:
	WeldJoint(Body* b0, Body* b1);

	virtual bool IsContain(const sm::vec2& pos) const;
	virtual bool IsIntersect(const ee::Rect& rect) const;

	virtual void Draw(DrawType type) const;

	sm::vec2 GetWorldAnchorA() const;
	sm::vec2 GetWorldAnchorB() const;

	void SetLocalAnchorA(const sm::vec2& world);
	void SetLocalAnchorB(const sm::vec2& world);

private:
	void DrawAnchor(const sm::vec2& pos, DrawType type) const;

public:
	sm::vec2 m_local_anchor_a;
	sm::vec2 m_local_anchor_b;

	float m_reference_angle;

	float m_frequency_hz;
	float m_damping_ratio;

}; // WeldJoint

}

#endif // _EASYMODELING_WELD_JOINT_H_