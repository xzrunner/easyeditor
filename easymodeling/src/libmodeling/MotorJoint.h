#ifndef _EASYMODELING_MOTOR_JOINT_H_
#define _EASYMODELING_MOTOR_JOINT_H_

#include "Joint.h"

#include <ee/Vector.h>

#include <vector>

namespace emodeling
{

class MotorJoint : public Joint
{
public:
	MotorJoint(Body* b0, Body* b1);

	virtual bool IsContain(const ee::Vector& pos) const;
	virtual bool IsIntersect(const ee::Rect& rect) const;

	virtual void Draw(DrawType type) const;

private:
	void DrawAnchor(const ee::Vector& pos, DrawType type) const;

public:
	ee::Vector m_linear_offset;
	float m_angular_offset;

	float m_max_force;

	float m_max_torque;

	float m_correction_factor;

}; // MotorJoint

}

#endif // _EASYMODELING_MOTOR_JOINT_H_