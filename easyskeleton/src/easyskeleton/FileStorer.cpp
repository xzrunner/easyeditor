#include "FileStorer.h"
#include "Joint.h"
#include "Bone.h"

#include <ee/FileHelper.h>
#include <ee/Sprite.h>
#include <ee/Symbol.h>

#include <gum/JointPoseIO.h>

#include <fstream>

namespace eskeleton
{

void FileStorer::Store(const std::string& filepath, const std::vector<ee::Sprite*>& sprs)
{
	Json::Value val;

	std::string dir = ee::FileHelper::GetFileDir(filepath) + "\\";
	val["sprite"] = StoreSprite(sprs, dir);

	val["skeleton"] = StoreSkeleton(sprs);	

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, val);
	fout.close();
}

Json::Value FileStorer::StoreSprite(const std::vector<ee::Sprite*>& sprs, const std::string& dir)
{
	Json::Value val;
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		const ee::Symbol* sym = dynamic_cast<const ee::Symbol*>(sprs[i]->GetSymbol());
		val[i]["filepath"] = ee::FileHelper::GetRelativePath(dir, sym->GetFilepath());
		sprs[i]->Store(val[i], dir);
	}
	return val;
}

Json::Value FileStorer::StoreSkeleton(const std::vector<ee::Sprite*>& sprs)
{
	std::map<const s2::Joint*, int> map_joint_id;
	int joint_id = 0;
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		Bone* bone = (Bone*)(sprs[i]->GetUserData());
		const std::vector<Joint*>& joints = bone->GetAllJoints();
		for (int j = 0, m = joints.size(); j < m; ++j) {
			map_joint_id.insert(std::make_pair(joints[j], joint_id++));
		}
	}

	Json::Value val;
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		Json::Value& bone_val = val[i];
		Bone* bone = (Bone*)(sprs[i]->GetUserData());
		const std::vector<Joint*>& joints = bone->GetAllJoints();
		for (int j = 0, m = joints.size(); j < m; ++j) {
			bone_val["joint"][j] = StoreJoint(joints[j], map_joint_id);
		}
	}

	return val;
}

Json::Value FileStorer::StoreJoint(const Joint* joint, const std::map<const s2::Joint*, int>& map_joint_id)
{
	Json::Value val;
	const s2::Joint* parent = joint->GetParent();
	if (parent) {
		std::map<const s2::Joint*, int>::const_iterator itr = map_joint_id.find(parent);
		assert(itr != map_joint_id.end());
		val["parent"] = itr->second;
	}

	gum::JointPoseIO::Store(val["world_pose"], joint->GetWorldPose());
	gum::JointPoseIO::Store(val["local_pose"], joint->GetLocalPose());
	gum::JointPoseIO::Store(val["skin_pose"], joint->GetSkinPose());
	return val;
}

}