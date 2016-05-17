#ifndef _EASYMODELING_WHEEL_JOINT_H_
#define _EASYMODELING_WHEEL_JOINT_H_

#include "Joint.h"

namespace emodeling
{

class WheelJoint : public Joint
{
public:
	WheelJoint(Body* b0, Body* b1);

	virtual bool IsContain(const sm::vec2& pos) const;
	virtual bool IsIntersect(const sm::rect& rect) const;

	virtual void Draw(DrawType type) const;

	sm::vec2 GetWorldAnchorA() const;
	sm::vec2 GetWorldAnchorB() const;

	void SetLocalAnchorA(const sm::vec2& world);
	void SetLocalAnchorB(const sm::vec2& world);

private:
	void DrawAxisALine(const sm::vec2& worldAnchorA) const;
	void DrawFootBLine(const sm::vec2& worldAnchorA, const sm::vec2& worldAnchorB) const;

	void DrawAnchorA(const sm::vec2& pos, DrawType type) const;
	void DrawAnchorB(const sm::vec2& pos, DrawType type) const;

public:
	sm::vec2 m_local_anchor_a;
	sm::vec2 m_local_anchor_b;

	sm::vec2 m_local_axis_a;

	bool m_enable_motor;
	float m_max_motor_torque;

	float m_motor_speed;

	float m_frequency_hz;
	float m_damping_ratio;

}; // WheelJoint

}

#endif // _EASYMODELING_WHEEL_JOINT_H_
