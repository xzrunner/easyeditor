//#pragma once
//
//#include <iostream>
//
//namespace ee
//{
//	struct JointDef
//	{
//		float positive_speed, negative_speed;
//		float left_angle_small, left_angle_large,
//			right_angle_small, right_angle_large;
//		float max_torque;
//
//		bool enableLimit, enableMotor;
//
//		JointDef();
//
//	}; // JointDef 
//
//	inline JointDef::JointDef()
//	{
//		positive_speed = 2.0f;
//		negative_speed = -2.0f;
//		max_torque = 0.0f;
//
//		enableLimit = enableMotor = true;
//	}
//
//	inline std::istream& operator >> (std::istream& is, JointDef& params)
//	{
//		is >> params.positive_speed >> params.negative_speed
//			>> params.left_angle_small >> params.left_angle_large
//			>> params.right_angle_small >> params.right_angle_large
//			>> params.max_torque;
//		return is;
//	}
//
//	inline std::ostream& operator << (std::ostream& os, const JointDef& params)
//	{
//		os << params.positive_speed << " " << params.negative_speed << " "
//			<< params.left_angle_small << " " << params.left_angle_large << " " 
//			<< params.right_angle_small << " " << params.right_angle_large << " "
//			<< params.max_torque;
//		return os;
//	}
//}
//
