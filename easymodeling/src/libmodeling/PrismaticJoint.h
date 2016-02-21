#ifndef _EASYMODELING_PRISMATIC_JOINT_H_
#define _EASYMODELING_PRISMATIC_JOINT_H_

#include "Joint.h"

#include <ee/Vector.h>

namespace emodeling
{

class PrismaticJoint : public Joint
{
public:
	PrismaticJoint(Body* b0, Body* b1);

	virtual bool IsContain(const ee::Vector& pos) const;
	virtual bool IsIntersect(const ee::Rect& rect) const;

	virtual void Draw(DrawType type) const;

	ee::Vector GetWorldAnchorA() const;
	ee::Vector GetWorldAnchorB() const;

	void SetLocalAnchorA(const ee::Vector& world);
	void SetLocalAnchorB(const ee::Vector& world);

private:
	void DrawAnchor(const ee::Vector& pos, DrawType type) const;

public:
	ee::Vector m_local_anchor_a;
	ee::Vector m_local_anchor_b;

	ee::Vector m_local_axis_a;

	float m_reference_angle;

	bool m_enable_limit;
	float m_lower_translation;
	float m_upper_translation;

	bool m_enable_motor;
	float m_max_motor_force;

	float m_motor_speed;

}; // PrismaticJoint

}

#endif // _EASYMODELING_PRISMATIC_JOINT_H_
