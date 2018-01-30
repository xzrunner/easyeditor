#include "s2loader/SkeletonIO.h"

#include <sprite2/Joint.h>
#include <sprite2/JointPose.h>
#include <sprite2/SkeletonPose.h>

namespace s2loader
{

/************************************************************************/
/* class JointPose                                                      */
/************************************************************************/

void SkeletonIO::Load(const Json::Value& val, s2::JointPose& pose)
{
	pose.trans.x	= static_cast<float>(val["trans_x"].asDouble());
	pose.trans.y	= static_cast<float>(val["trans_y"].asDouble());
	pose.rot		= static_cast<float>(val["rot"].asDouble());
	pose.scale.x	= static_cast<float>(val["scale_x"].asDouble());
	pose.scale.y	= static_cast<float>(val["scale_y"].asDouble());
}

void SkeletonIO::Store(Json::Value& val, const s2::JointPose& pose)
{
	val["trans_x"]	= pose.trans.x;
	val["trans_y"]	= pose.trans.y;
	val["rot"]		= pose.rot;
	val["scale_x"]	= pose.scale.x;
	val["scale_y"]	= pose.scale.y;
}

/************************************************************************/
/* class SkeletonPose                                                   */
/************************************************************************/

void SkeletonIO::Load(const Json::Value& val, s2::SkeletonPose& pose)
{
	CU_VEC<s2::JointPose> joints;
	joints.reserve(val["joint"].size());
	for (int i = 0, n = val["joint"].size(); i < n; ++i) {
		s2::JointPose joint;
		Load(val["joint"][i], joint);
		joints.push_back(joint);
	}
	pose.SetJointPose(joints);
}

void SkeletonIO::Store(Json::Value& val, const s2::SkeletonPose& pose)
{
	const CU_VEC<s2::JointPose>& joints = pose.GetJointPose();
	for (int i = 0, n = joints.size(); i < n; ++i) {
		Store(val["joint"][i], joints[i]);
	}
}

}