#include "s2loader/EasySkeletonLoader.h"
#include "s2loader/SpriteLoader.h"
#include "s2loader/JointLoader.h"
#include "s2loader/SpriteFactory.h"
#include "s2loader/SkeletonIO.h"

#include <SM_Calc.h>
#include <sprite2/SkeletonSymbol.h>
#include <sprite2/Sprite.h>
#include <sprite2/Joint.h>
#include <sprite2/Skeleton.h>

#include <queue>

#include <assert.h>

namespace s2loader
{

EasySkeletonLoader::EasySkeletonLoader(s2::SkeletonSymbol& sym,
									   const std::shared_ptr<const SpriteLoader>& spr_loader,
									   const std::shared_ptr<const JointLoader>& joint_loader)
	: m_sym(sym)
	, m_spr_loader(spr_loader)
	, m_joint_loader(joint_loader)
	, m_num(0)
{
	if (!m_spr_loader) {
		m_spr_loader = std::make_shared<SpriteLoader>();
	}
	if (!m_joint_loader) {
		m_joint_loader = std::make_shared<JointLoader>();
	}
}

EasySkeletonLoader::~EasySkeletonLoader()
{
	Clear();
}

void EasySkeletonLoader::LoadJson(const Json::Value& val, const CU_STR& dir)
{
	Clear();

	m_num = val["skeleton"].size();
	assert(m_num == val["sprite"].size());	
	if (m_num == 0) {
		return;
	}

	LoadSprites(val["sprite"], dir);
	LoadJoints(val["skeleton"]);
	InitRoot();
	InitPose();

	std::unique_ptr<s2::Skeleton> skeleton(std::make_unique<s2::Skeleton>(m_root, m_joints));
	m_sym.SetSkeleton(skeleton);
}

void EasySkeletonLoader::Clear()
{
	m_num = 0;

	m_sprs.clear();

	m_joints.clear();

	m_root.reset();
}

void EasySkeletonLoader::LoadSprites(const Json::Value& val, const CU_STR& dir)
{
	m_sprs.reserve(m_num);
	for (int i = 0; i < m_num; ++i) {
		m_sprs.push_back(m_spr_loader->Create(val[i], dir));
	}
}

void EasySkeletonLoader::LoadJoints(const Json::Value& val)
{
	m_joints.reserve(m_num);

	CU_VEC<Joint> src_joints;
	for (int i = 0; i < m_num; ++i) 
	{
		auto spr = m_sprs[i];
		const Json::Value& joints_val = val[i]["joint"];
		std::shared_ptr<s2::Joint> dst_joint = nullptr;
		for (int j = 0, m = joints_val.size(); j < m; ++j) 
		{
			const Json::Value& joint_val = joints_val[j];
			Joint src_joint;
			SkeletonIO::Load(joint_val["world_pose"], src_joint.world);
			SkeletonIO::Load(joint_val["skin_pose"], src_joint.skin);
			src_joint.idx = i;
			if (joint_val.isMember("parent")) {
				src_joint.parent = joint_val["parent"].asInt();
				if (!dst_joint) {
					dst_joint = m_joint_loader->Create(spr, -src_joint.skin);
					dst_joint->SetWorldPose(src_joint.world);
				}
			}
			src_joints.push_back(src_joint);
		}
		if (!dst_joint) {
			dst_joint = m_joint_loader->Create(spr, s2::JointPose());
			dst_joint->SetWorldPose(s2::JointPose(spr->GetCenter(), spr->GetAngle(), spr->GetScale()));
		}
		m_joints.push_back(dst_joint);
	}

	// connect
	for (int i = 0, n = src_joints.size(); i < n; ++i) {
		Joint src_joint = src_joints[i];
		if (src_joint.parent != -1) {
			m_joints[src_joints[src_joint.parent].idx]->ConnectChild(m_joints[src_joint.idx]);
		}
	}
}

void EasySkeletonLoader::InitRoot()
{
	assert(!m_joints.empty());
	m_root = m_joints[0];
	while (auto parent = m_root->GetParent()) {
		m_root = parent;
	}
}

void EasySkeletonLoader::InitPose()
{
	// world
	auto& children = m_root->GetChildren();
	for (int i = 0, n = children.size(); i < n; ++i) 
	{
		auto& child = children[i];
		float rot = sm::get_line_angle(m_root->GetWorldPose().trans, child->GetWorldPose().trans);
		std::queue<std::shared_ptr<s2::Joint>> buf;
		buf.push(child);
		while (!buf.empty()) {
			auto joint = buf.front(); buf.pop();
			s2::JointPose world = joint->GetWorldPose();
			world.rot += rot;
			joint->SetWorldPose(world);
			auto& children = joint->GetChildren();
			for (int i = 0, n = children.size(); i < n; ++i) {
				buf.push(children[i]);
			}
		}
	}

	// local
	std::queue<std::shared_ptr<s2::Joint>> buf;
	for (int i = 0, n = children.size(); i < n; ++i) {
		buf.push(children[i]);
	}
	while (!buf.empty()) 
	{
		auto joint = buf.front(); buf.pop();
		assert(joint->GetParent());
		joint->SetLocalPose(s2::world2local(joint->GetParent()->GetWorldPose(), joint->GetWorldPose()));
		auto& skin = joint->GetSkinSpr();
		s2::JointPose local(skin->GetCenter(), skin->GetAngle(), skin->GetScale());
		joint->SetSkinPose(s2::world2local(joint->GetWorldPose(), local));
		auto& children = joint->GetChildren();
		for (int i = 0, n = children.size(); i < n; ++i) {
			buf.push(children[i]);
		}
	}
}

}