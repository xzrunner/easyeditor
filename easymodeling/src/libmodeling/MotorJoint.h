#ifndef _EASYMODELING_MOTOR_JOINT_H_
#define _EASYMODELING_MOTOR_JOINT_H_

#include "Joint.h"

#include <vector>

namespace emodeling
{

class MotorJoint : public Joint
{
public:
	MotorJoint(Body* b0, Body* b1);

	virtual bool IsContain(const sm::vec2& pos) const;
	virtual bool IsIntersect(const sm::rect& rect) const;

	virtual void Draw(DrawType type) const;

private:
	void DrawAnchor(const sm::vec2& pos, DrawType type) const;

public:
	sm::vec2 m_linear_offset;
	float m_angular_offset;

	float m_max_force;

	float m_max_torque;

	float m_correction_factor;

}; // MotorJoint

}

#endif // _EASYMODELING_MOTOR_JOINT_H_