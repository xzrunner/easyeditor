#include "FileStorer.h"
#include "Joint.h"
#include "Bone.h"

#include <ee/FileHelper.h>
#include <ee/Sprite.h>
#include <ee/Symbol.h>

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
	std::map<const Joint*, int> map_joint_id;
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

Json::Value FileStorer::StoreJoint(const Joint* joint, const std::map<const Joint*, int>& map_joint_id)
{
	Json::Value val;
	const Joint* parent = joint->GetParent();
	if (parent) {
		std::map<const Joint*, int>::const_iterator itr = map_joint_id.find(parent);
		assert(itr != map_joint_id.end());
		val["parent"] = itr->second;
	}
	val["world_pose"] = StoreJointPose(joint->GetWorldPose());
	val["local_pose"] = StoreJointPose(joint->GetLocalPose());
	val["skin_pose"] = StoreJointPose(joint->GetSkinPose());
	return val;
}

Json::Value FileStorer::StoreJointPose(const s2::JointPose& pose)
{
	Json::Value val;
	val["trans_x"] = pose.trans.x;
	val["trans_y"] = pose.trans.y;
	val["rot"] = pose.rot;
	val["scale"] = pose.scale;
	return val;
}

}