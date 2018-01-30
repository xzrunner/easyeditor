#ifndef _S2LOADER_SKELETON_IO_H_
#define _S2LOADER_SKELETON_IO_H_

#include <json/json.h>

namespace s2 { class JointPose; class SkeletonPose; }

namespace s2loader
{

class SkeletonIO
{
public:
	// JointPose
	static void Load(const Json::Value& val, s2::JointPose& pose);
	static void Store(Json::Value& val, const s2::JointPose& pose);

	// SkeletonPose
	static void Load(const Json::Value& val, s2::SkeletonPose& pose);
	static void Store(Json::Value& val, const s2::SkeletonPose& pose);

}; // SkeletonIO

}

#endif // _S2LOADER_SKELETON_IO_H_
