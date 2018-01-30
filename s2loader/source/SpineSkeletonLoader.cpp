#include "s2loader/SpineSkeletonLoader.h"
#include "s2loader/SpriteLoader.h"
#include "s2loader/JointLoader.h"
#include "s2loader/SpineParser.h"

#include <sprite2/SkeletonSymbol.h>
#include <sprite2/Sprite.h>
#include <sprite2/Joint.h>
#include <sprite2/Skeleton.h>
#include <gum/FilepathHelper.h>

#include <assert.h>

namespace s2loader
{

SpineSkeletonLoader::SpineSkeletonLoader(s2::SkeletonSymbol& sym,
										 const std::shared_ptr<const SpriteLoader>& spr_loader,
										 const std::shared_ptr<const JointLoader>& joint_loader)
	: m_sym(sym)
	, m_spr_loader(spr_loader)
	, m_joint_loader(joint_loader)
{
	if (!m_spr_loader) {
		m_spr_loader = std::make_shared<SpriteLoader>();
	}
	if (!m_joint_loader) {
		m_joint_loader = std::make_shared<JointLoader>();
	}
}

void SpineSkeletonLoader::LoadJson(const Json::Value& val, const CU_STR& dir)
{
	SpineParser parser(false);
	parser.Parse(val);
	LoadParser(parser, dir);
}

void SpineSkeletonLoader::LoadParser(const SpineParser& parser, const CU_STR& dir)
{
	Clear();

	m_num = parser.bones.size();

	CU_STR img_dir = gum::FilepathHelper::Absolute(dir, parser.img_dir);
	LoadSprites(parser, img_dir);
	LoadJoints(parser);
	InitRoot();
	InitPose(parser);
	InitSkeleton(parser);
}

void SpineSkeletonLoader::Clear()
{
	m_num = 0;

	m_sprs.clear();

	m_joints.clear();

	m_root.reset();
}

void SpineSkeletonLoader::LoadSprites(const SpineParser& parser, const CU_STR& img_dir)
{
	m_sprs.reserve(m_num);
	for (int i = 0, n = parser.slots.size(); i < n; ++i)
	{
		const SpineParser::Slot& slot = parser.slots[i];
		const SpineParser::SkinItem* item = parser.QuerySkin(slot);
		if (!item) {
			m_sprs.push_back(nullptr);
			continue;
		}
		CU_STR filepath = gum::FilepathHelper::Absolute(img_dir, item->name + ".png");
		auto spr = m_spr_loader->Create(filepath);
		spr->SetAngle(item->angle);
		spr->SetPosition(item->pos);
		m_sprs.push_back(spr);
	}
}

void SpineSkeletonLoader::LoadJoints(const SpineParser& parser)
{
	for (int i = 0, n = parser.slots.size(); i < n; ++i)
	{
		const SpineParser::Slot& slot = parser.slots[i];
		auto& spr = m_sprs[i];
		if (!spr) {
			continue;
		}
		s2::JointPose pose(spr->GetPosition(), spr->GetAngle(), spr->GetScale());
		auto joint = m_joint_loader->Create(spr, -pose);
		m_joints.insert(std::make_pair(slot.bone, joint));
	}

	for (int i = 0, n = parser.bones.size(); i < n; ++i) {
		const SpineParser::Bone& bone = parser.bones[i];
		auto itr_joint = m_joints.find(bone.name);
		if (itr_joint != m_joints.end()) {
			continue;
		}
		auto joint = m_joint_loader->Create(nullptr, s2::JointPose());
		m_joints.insert(std::make_pair(bone.name, joint));
	}

	// connect
	for (int i = 0, n = parser.bones.size(); i < n; ++i)
	{
		const SpineParser::Bone& bone = parser.bones[i];
		if (bone.parent.empty()) {
			continue;
		}

		auto itr_joint = m_joints.find(bone.name);
		assert(itr_joint != m_joints.end());
		auto& child = itr_joint->second;
		itr_joint = m_joints.find(bone.parent);
		assert(itr_joint != m_joints.end());
		auto& parent = itr_joint->second;
		parent->ConnectChild(child);
	}
}

void SpineSkeletonLoader::InitRoot()
{
	assert(!m_joints.empty());
	auto root = m_joints.begin()->second;
	while (auto parent = root->GetParent()) {
		root = parent;
	}
	m_root = root;
}

void SpineSkeletonLoader::InitPose(const SpineParser& parser)
{
	auto itr = m_joints.begin();
	for ( ; itr != m_joints.end(); ++itr)
	{
		const SpineParser::Bone* src = nullptr;
		for (int i = 0, n = parser.bones.size(); i < n; ++i) {
			if (parser.bones[i].name == itr->first) {
				src = &parser.bones[i];
				break;
			}			
		}
		assert(src);

		auto dst = itr->second;
		dst->SetLocalPose(s2::JointPose(src->pos, src->angle, src->scale));
	}
	m_root->Update();
}

void SpineSkeletonLoader::InitSkeleton(const SpineParser& parser)
{
	CU_VEC<std::shared_ptr<s2::Joint>> joints;
	joints.reserve(m_joints.size());
	for (int i = 0, n = parser.slots.size(); i < n; ++i) 
	{
		auto itr = m_joints.find(parser.slots[i].bone);
		assert(itr != m_joints.end());
		auto joint = itr->second;
		if (joint->GetName().empty()) {
			joint->SetName("*");
			joints.push_back(joint);
		}
	}

	auto itr = m_joints.begin();
	for ( ; itr != m_joints.end(); ++itr) 
	{
		auto joint = itr->second;
		if (joint->GetName().empty()) {
			joints.push_back(joint);
		}
		joint->SetName(itr->first);
	}

	auto skeleton(std::make_unique<s2::Skeleton>(m_root, joints));
	m_sym.SetSkeleton(skeleton);
}

}