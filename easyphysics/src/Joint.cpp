//#include "Joint.h"
//#include "JointDef.h"
//
//#include <Box2D/Box2D.h>
//
//#include "common/d2d::Math.h"
//
//namespace ephysics
//{
//
//Joint::Joint(b2World* world, const JointDef& params, const d2d::Vector& anchor, 
//			 b2Body* b0, b2Body* b1, bool isMotor, float scale)
//	: m_world(world), m_joint(NULL)
//{
//	m_scale = scale;
//	initialize(params, anchor, b0, b1, isMotor);
//}
//
//Joint::~Joint()
//{
//	if (m_joint)
//	{
//		m_world->DestroyJoint(m_joint);
//		m_joint = NULL;
//	}
//}
//
//void Joint::setMotorSpeed(bool isPositive)
//{
//	if (isPositive) m_joint->SetMotorSpeed(m_positiveSpeed);
//	else m_joint->SetMotorSpeed(m_negativeSpeed);
//}
//
//void Joint::setAngleLimit(bool isLeft)
//{
//	float lower, upper;
//	if (isLeft)
//	{
//		lower = std::min(m_leftAngleSmall, m_leftAngleLarge);
//		upper = std::max(m_leftAngleSmall, m_leftAngleLarge);
//	}
//	else
//	{
//		lower = std::min(m_rightAngleSmall, m_rightAngleLarge);
//		upper = std::max(m_rightAngleSmall, m_rightAngleLarge);
//	}
//	lower *= PI;
//	upper *= PI;
//
//	m_joint->SetLimits(lower, upper);
//}
//
//void Joint::initialize(const JointDef& params, const d2d::Vector& anchor, b2Body* b0, b2Body* b1, bool isMotor)
//{
//	m_negativeSpeed = params.negative_speed;
//	m_positiveSpeed = params.positive_speed;
//
//	m_leftAngleSmall = params.left_angle_small;
//	m_leftAngleLarge = params.left_angle_large;
//	m_rightAngleSmall = params.right_angle_small;
//	m_rightAngleLarge = params.right_angle_large;
//
//	b2RevoluteJointDef rjd;
//	rjd.Initialize(b0, b1, b2Vec2(anchor.x / BOX2D_SCALE_FACTOR, anchor.y / BOX2D_SCALE_FACTOR));
//	rjd.collideConnected = false;
//	rjd.maxMotorTorque = params.max_torque;
// 	if (isMotor)
// 	{
// 		rjd.enableLimit = true;
// 		rjd.enableMotor = true;
// 	}
// 	else
//	{
//		rjd.enableLimit = false;
//		rjd.enableMotor = false;
//	}
//
//	if (m_joint) m_world->DestroyJoint(m_joint);
//
//	m_joint = (b2RevoluteJoint*) m_world->CreateJoint(&rjd);
//}
//
//}
