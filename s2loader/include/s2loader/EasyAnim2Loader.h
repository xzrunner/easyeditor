#ifndef _S2LOADER_EASY_ANIM2_LOADER_H_
#define _S2LOADER_EASY_ANIM2_LOADER_H_

#include <cu/uncopyable.h>
#include <sprite2/JointPose.h>
#include <sprite2/typedef.h>
#include <cu/cu_stl.h>

#include <json/json.h>

struct rg_joint;
struct rg_pose_srt;
struct rg_skeleton;

namespace s2 { class Anim2Symbol; class Sprite; }

namespace s2loader
{

class SymbolLoader;
class JointLoader;

class EasyAnim2Loader : private cu::Uncopyable
{
public:
	EasyAnim2Loader(s2::Anim2Symbol& sym, 
		const std::shared_ptr<const SymbolLoader>& sym_loader = nullptr);
	~EasyAnim2Loader();

	void LoadJson(const Json::Value& val, const CU_STR& dir);

private:
	void Clear();

	void LoadSprites(const Json::Value& val, const CU_STR& dir);
	void LoadJoints(const Json::Value& val);
	void InitRoot();
	void InitPose();

	static void CopyJointPose(rg_pose_srt* dst, const s2::JointPose& src);

private:
	struct Joint
	{
		s2::JointPose world_pose;
		s2::JointPose skin_pose;

		s2::SprPtr skin = nullptr;

		int parent;
		CU_VEC<int> children;

		int idx;

		Joint() : parent(-1), idx(-1) {}
	};

private:
	s2::Anim2Symbol& m_sym;

	std::shared_ptr<const SymbolLoader> m_sym_loader;

	int m_num;
	CU_VEC<s2::SprPtr> m_sprs;

	int          m_joint_count;
	rg_joint**   m_joints;
	rg_skeleton* m_sk;
	rg_joint*    m_root;

}; // EasyAnim2Loader

}

#endif // _S2LOADER_EASY_ANIM2_LOADER_H_
