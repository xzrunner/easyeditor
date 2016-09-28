#include "FileLoader.h"
#include "Joint.h"
#include "Bone.h"

#include <ee/FileHelper.h>
#include <ee/SpriteFactory.h>
#include <ee/Sprite.h>

#include <gum/SkeletonSymLoader.h>

#include <fstream>

namespace eskeleton
{

void FileLoader::Load(const std::string& filepath, std::vector<ee::Sprite*>& sprs)
{
	Json::Value val;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, val);
	fin.close();

	std::string dir = ee::FileHelper::GetFileDir(filepath);
	LoadSprite(dir, val["sprite"], sprs);

	LoadSkeleton(val["skeleton"], sprs);
}

void FileLoader::LoadSprite(const std::string& dir, const Json::Value& val, std::vector<ee::Sprite*>& sprs)
{
	for (int i = 0, n = val.size(); i < n; ++i) {
		sprs.push_back(ee::SpriteFactory::Instance()->Create(val[i], dir));
	}
}

void FileLoader::LoadSkeleton(const Json::Value& val, const std::vector<ee::Sprite*>& sprs)
{
	assert(val.size() == sprs.size());

	std::vector<Joint*> joints;

	for (int i = 0, n = val.size(); i < n; ++i) {
		Bone* bone = new Bone;
		bone->SetSkin(sprs[i]);
		const Json::Value& joint_val = val[i]["joint"];
		for (int j = 0, m = joint_val.size(); j < m; ++j) {
			s2::JointPose skin_pose;
			gum::SkeletonSymLoader::LoadJointPose(joint_val[j]["skin_pose"], skin_pose);
			Joint* joint = new Joint(sprs[i], -skin_pose.trans);
			bone->AddJoint(joint);
			joints.push_back(joint);
		}
		sprs[i]->SetUserData(bone);
	}

	int idx = 0;
	for (int i = 0, n = val.size(); i < n; ++i) {
		const Json::Value& joint_val = val[i]["joint"];
		for (int j = 0, m = joint_val.size(); j < m; ++j) {
			Joint* joint = joints[idx++];

			s2::JointPose world_pose, local_pose;
			gum::SkeletonSymLoader::LoadJointPose(joint_val[j]["world_pose"], world_pose);
			gum::SkeletonSymLoader::LoadJointPose(joint_val[j]["local_pose"], local_pose);
			joint->SetWorldPose(world_pose);
			joint->SetLocalPose(local_pose);

			if (joint_val[j].isMember("parent")) {
				int id = joint_val[j]["parent"].asInt();
				joints[id]->Connect(joint);
			}
		}
	}
}

}