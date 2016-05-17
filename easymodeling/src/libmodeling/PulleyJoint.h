#ifndef _EASYMODELING_PULLEY_JOINT_H_
#define _EASYMODELING_PULLEY_JOINT_H_

#include "Joint.h"

namespace emodeling
{

class PulleyJoint : public Joint
{
public:
	PulleyJoint(Body* b0, Body* b1);

	virtual bool IsContain(const sm::vec2& pos) const;
	virtual bool IsIntersect(const sm::rect& rect) const;

	virtual void Draw(DrawType type) const;

	sm::vec2 GetWorldAnchorA() const;
	sm::vec2 GetWorldAnchorB() const;

	void SetLocalAnchorA(const sm::vec2& world);
	void SetLocalAnchorB(const sm::vec2& world);

private:
	void DrawAnchor(const sm::vec2& pos, DrawType type) const;

public:
	sm::vec2 m_ground_anchor_a;
	sm::vec2 m_ground_anchor_b;

	sm::vec2 m_local_anchor_a;
	sm::vec2 m_local_anchor_b;

	float m_ratio;

}; // PulleyJoint

}

#endif // _EASYMODELING_PULLEY_JOINT_H_
