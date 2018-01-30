#ifndef _S2LOADER_SPINE_ANIM2_LOADER_H_
#define _S2LOADER_SPINE_ANIM2_LOADER_H_

#include "s2loader/SpineParser.h"

#include <cu/uncopyable.h>
#include <rigging.h>

#include <json/value.h>

#include <memory>

namespace s2 { class Anim2Symbol; class Symbol; }

struct rg_joint;
struct rg_skeleton;
struct rg_tl_joint;
struct rg_tl_skin;
struct rg_tl_deform;

namespace s2loader
{

class SymbolLoader;
class SpineParser;

class SpineAnim2Loader : private cu::Uncopyable
{
public:
	SpineAnim2Loader(s2::Anim2Symbol& sym, 
		const std::shared_ptr<const SymbolLoader>& sym_loader = nullptr);
	~SpineAnim2Loader();

	void LoadJson(const Json::Value& val, const CU_STR& dir);

	void LoadParser(const SpineParser& parser, const CU_STR& dir);

private:
	void Clear();

	void LoadJointsData(const SpineParser& parser);
	void ConnectJoints(const SpineParser& parser);

	void CreateSkins(const SpineParser& parser, const CU_STR& img_dir);
	void CreateImageSkin(rg_skin& dst, const SpineParser::SkinItem& src, const CU_STR& img_dir) const;
	void CreateMeshSkin(rg_skin& dst, const SpineParser::SkinItem& src, const CU_STR& img_dir) const;
	void CreateSlots(const SpineParser& parser);
	void CreateJoints();
	void CreateIKs(const SpineParser& parser);
	void CreateSkeleton();
	void InitRoot();
 	void InitPose(const SpineParser& parser);

	void LoadTimelineJoints(const SpineParser::Animation& anim);
	void LoadTimelineJoints(const SpineParser::AnimBone* bone, struct rg_tl_joint* joint);
	
	void LoadTimelineSkins(const SpineParser::Animation& anim);
	void LoadTimelineSkins(const SpineParser::AnimSlot* slot, struct rg_tl_skin* skin);

	void LoadTimelineDeforms(const SpineParser::Animation& anim);
	void LoadTimelineDeforms(const SpineParser::AnimDeform* deform);

	void LoadCurves(const CU_VEC<SpineParser::Curve>& curves);

private:
	struct JointData
	{
		CU_STR name;

		CU_VEC<int> children;

		JointData(const CU_STR& name) 
			: name(name) {}
	};

	struct SlotData
	{
		CU_STR name;

		SlotData(const CU_STR& name) 
			: name(name) {}
	};

	struct SkinData
	{
		CU_STR slot;
		CU_STR name;

		SkinData(const CU_STR& slot, const CU_STR& name)
			: slot(slot), name(name) {}
	};

private:
	s2::Anim2Symbol& m_sym;

	std::shared_ptr<const SymbolLoader> m_sym_loader;

	int m_num;

	// middle
	CU_VEC<JointData>   m_joints_data;
	CU_VEC<SlotData>    m_slots_data;
	CU_VEC<SkinData>    m_skins_data;
	CU_MAP<CU_STR, int> m_bone2joint;
	CU_MAP<CU_STR, int> m_map2skin;

	// dst
	CU_VEC<rg_ik>   m_iks;
	CU_VEC<rg_skin> m_skins;
	CU_VEC<rg_slot> m_slots;
	int             m_joint_count;
	rg_joint**      m_joints;
	rg_skeleton*    m_sk;
	rg_joint*       m_root;
	rg_tl_joint**   m_tl_joints;
	rg_tl_skin**    m_tl_skins;
	rg_tl_deform**  m_tl_deforms;
	int             m_max_frame;
	rg_curve**      m_curves;

}; // SpineAnim2Loader

}

#endif // _S2LOADER_SPINE_ANIM2_LOADER_H_
