#ifndef _EASYMODELING_WHEEL_JOINT_H_
#define _EASYMODELING_WHEEL_JOINT_H_

#include "Joint.h"

#include <ee/Vector.h>

namespace emodeling
{

class WheelJoint : public Joint
{
public:
	WheelJoint(Body* b0, Body* b1);

	virtual bool IsContain(const ee::Vector& pos) const;
	virtual bool IsIntersect(const ee::Rect& rect) const;

	virtual void Draw(DrawType type) const;

	ee::Vector GetWorldAnchorA() const;
	ee::Vector GetWorldAnchorB() const;

	void SetLocalAnchorA(const ee::Vector& world);
	void SetLocalAnchorB(const ee::Vector& world);

private:
	void DrawAxisALine(const ee::Vector& worldAnchorA) const;
	void DrawFootBLine(const ee::Vector& worldAnchorA, const ee::Vector& worldAnchorB) const;

	void DrawAnchorA(const ee::Vector& pos, DrawType type) const;
	void DrawAnchorB(const ee::Vector& pos, DrawType type) const;

public:
	ee::Vector m_local_anchor_a;
	ee::Vector m_local_anchor_b;

	ee::Vector m_local_axis_a;

	bool m_enable_motor;
	float m_max_motor_torque;

	float m_motor_speed;

	float m_frequency_hz;
	float m_damping_ratio;

}; // WheelJoint

}

#endif // _EASYMODELING_WHEEL_JOINT_H_
