#include "SkeletonData.h"

#include "dataset/Joint.h"

#include <ee/Sprite.h>
#include <ee/panel_msg.h>

#include <queue>

#include <assert.h>

namespace eanim
{

SkeletonData::~SkeletonData()
{
	Clean();
}

void SkeletonData::CopyFrom(const SkeletonData& skeleton)
{
	std::vector<ee::SprPtr> sprs;
	sprs.reserve(skeleton.m_map_joints.size());
	for (auto& item : skeleton.m_map_joints) {
		sprs.push_back(std::dynamic_pointer_cast<ee::Sprite>(item.first->Clone()));
	}
	CopyFrom(sprs, skeleton);
}

void SkeletonData::CopyFrom(const std::vector<ee::SprPtr>& sprs,
	const SkeletonData& skeleton)
{
	Clean();

	// check
	for (auto& item : skeleton.m_map_joints) {
		if (!GetSpriteByName(sprs, item.first->GetName())) {
			return;
		}
	}

	// prepare
	std::map<Joint*, Joint*> mapCovJoint;
	for (auto& item : skeleton.m_map_joints)
	{
		auto spr = GetSpriteByName(sprs, item.first->GetName());
		if (!spr) {
			return;
		}

		for (int i = 0, n = item.second.size(); i < n; ++i)
		{
			Joint* src = item.second[i];
			if (mapCovJoint.find(src) != mapCovJoint.end())
				continue;

			Joint* dst = new Joint(spr);
			dst->m_relative_pos = src->m_relative_pos;
			dst->m_relative_angle = src->m_relative_angle;
			mapCovJoint.insert(std::make_pair(src, dst));
		}
	}

	// convert joint
	for (auto& item : mapCovJoint)
	{
		Joint* src = item.first;
		Joint* dst = item.second;

		// parent
		if (src->m_parent == NULL)
		{
			dst->m_parent = NULL;
		}
		else
		{
			std::map<Joint*, Joint*>::iterator itr_parent = mapCovJoint.find(src->m_parent);
			assert(itr_parent != mapCovJoint.end());
			dst->m_parent = itr_parent->second;
		}

		// children
		std::set<Joint*>::iterator itr_child = src->m_children.begin();
		for ( ; itr_child != src->m_children.end(); ++itr_child)
		{
			std::map<Joint*, Joint*>::iterator find = mapCovJoint.find(*itr_child);
			assert(find != mapCovJoint.end());
			dst->m_children.insert(find->second);
		}
	}

	// convert skeleton
	for (auto& item : skeleton.m_map_joints)
	{
		auto& src_sprite = item.first;
		std::vector<Joint*> src_joints = item.second;

		auto dst_sprite = GetSpriteByName(sprs, src_sprite->GetName());
		std::vector<Joint*> dst_joints;
		for (int i = 0, n = src_joints.size(); i < n; ++i)
		{
			std::map<Joint*, Joint*>::iterator find = mapCovJoint.find(src_joints[i]);
			assert(find != mapCovJoint.end());
			dst_joints.push_back(find->second);
		}

		m_map_joints.insert(std::make_pair(dst_sprite, dst_joints));
	}
}

void SkeletonData::RemoveSprite(const ee::SprPtr& spr)
{
	auto itr = m_map_joints.find(spr);
	if (itr != m_map_joints.end())
	{
		for (int i = 0, n = itr->second.size(); i < n; ++i)
			delete itr->second[i];
		m_map_joints.erase(itr);
	}
}

bool SkeletonData::IsContainSprite(const ee::SprPtr& spr) const
{
	return m_map_joints.find(spr) != m_map_joints.end();
}

void SkeletonData::InsertJoint(const ee::SprPtr& spr, const sm::vec2& pos)
{
	auto itr = m_map_joints.find(spr);
	if (itr == m_map_joints.end())
	{
		std::vector<Joint*> joints;
		joints.push_back(new Joint(spr, pos));
		m_map_joints.insert(std::make_pair(spr, joints));
	}
	else
	{
		for (int i = 0, n = itr->second.size(); i < n; ++i) {
			if (itr->second[i]->Contain(pos)) {
				return;
			}
		}
		itr->second.push_back(new Joint(itr->first, pos));
	}
}

void SkeletonData::RemoveJoint(sm::vec2& pos)
{
	for (auto& item : m_map_joints)
	{
		std::vector<Joint*>::iterator itr_joint = item.second.begin();
		for ( ; itr_joint != item.second.end(); ++itr_joint)
		{
			Joint* joint = *itr_joint;
			if (joint->Contain(pos))
			{
				delete joint;
				item.second.erase(itr_joint);
				return;
			}
		}
	}
}

Joint* SkeletonData::QueryJointByPos(const sm::vec2& pos)
{
	for (auto& item : m_map_joints)
	{
		std::vector<Joint*>::iterator itr_joint = item.second.begin();
		for ( ; itr_joint != item.second.end(); ++itr_joint) {
			if ((*itr_joint)->Contain(pos)) {
				return *itr_joint;
			}
		}
	}
	return NULL;
}

void SkeletonData::Draw() const
{
	for (auto& item : m_map_joints) {
		for (int i = 0, n = item.second.size(); i < n; ++i) {
			item.second[i]->Draw();
		}
	}
}

void SkeletonData::Absorb(const ee::SprPtr& spr)
{
	auto itr_child = m_map_joints.find(spr);
	if (itr_child == m_map_joints.end())
		return;

	for (auto& item : m_map_joints)
	{
		if (itr_child->first == item.first)
			continue;
		for (int i = 0, n = itr_child->second.size(); i < n; ++i)
		{
			Joint* c = itr_child->second[i];
			sm::vec2 cp = c->GetWorldPos();
			for (int j = 0, m = item.second.size(); j < m; ++j)
			{					
				Joint* p = item.second[j];
				if (p->Intersect(cp))
				{
					sm::vec2 pp = p->GetWorldPos();
					bool success = p->Connect(c);
					if (success) {
						Translate(c->m_spr, pp - cp);
						return;
					}
				}
			}
			c->Deconnect();
		}
	}
}

void SkeletonData::FixJoint(const ee::SprPtr& spr)
{
	auto itr = m_map_joints.find(spr);
	if (itr == m_map_joints.end()) {
		return;
	}

	for (int i = 0, n = itr->second.size(); i < n; ++i)
	{
		Joint* j = itr->second[i];
		if (j->m_parent)
		{
			spr->Translate(j->m_parent->GetWorldPos() - j->GetWorldPos());
			break;
		}
	}
}

void SkeletonData::UpdateJoint(const ee::SprPtr& spr, float dAngle)
{
	auto itr = m_map_joints.find(spr);
	if (itr == m_map_joints.end()) {
		return;
	}

	for (int i = 0, n = itr->second.size(); i < n; ++i)
	{
		Joint* p = itr->second[i];
		sm::vec2 pp = p->GetWorldPos();
		std::set<Joint*>::iterator itr_joint = p->m_children.begin();
		for ( ; itr_joint != p->m_children.end(); ++itr_joint)
		{
			Joint* c = *itr_joint;
			sm::vec2 cp = c->GetWorldPos();
			c->m_spr->Translate(pp - cp);
			if (dAngle != 0)
				c->m_spr->Rotate(dAngle);
			UpdateJoint(c->m_spr, dAngle);
		}
	}
}

void SkeletonData::GetTweenSprites(SkeletonData& start, SkeletonData& end, 
								   std::vector<ee::SprPtr>& tween, int time, int tot_time)
{
	float process = static_cast<float>(time) / tot_time;

	SkeletonData mid;
	mid.CopyFrom(start);

	for (auto& itr_s : start.m_map_joints)
	{
		auto& s = itr_s.first;
		for (auto& itr_e : end.m_map_joints)
		{
			if (s->GetName() == itr_e.first->GetName())
			{
				auto& e = itr_e.first;
				
				ee::SprPtr spr = nullptr;
				for (auto& itr_mid : mid.m_map_joints)
				{
					if (itr_mid.first->GetName() == s->GetName())
					{
						spr = itr_mid.first;
						break;
					}
				}
				assert(spr);

				float delta = (e->GetAngle() - s->GetAngle()) * process;
				spr->Rotate(delta);
				mid.FixJoint(spr);

				break;
			}
		}
	}

	for (auto& item : mid.m_map_joints) {
		tween.push_back(std::dynamic_pointer_cast<ee::Sprite>(item.first));
	}
}

void SkeletonData::Clean()
{
	for (auto& item : m_map_joints) {
		for (int i = 0, n = item.second.size(); i < n; ++i) {
			delete item.second[i];
		}
	}
	m_map_joints.clear();
}

void SkeletonData::Translate(const ee::SprPtr& spr, const sm::vec2& offset)
{
	std::set<ee::SprPtr> sprs;
	std::queue<ee::SprPtr> buf;
	buf.push(spr);
	while (!buf.empty())
	{
		auto curr = buf.front(); buf.pop();
		sprs.insert(curr);

		auto itr = m_map_joints.find(curr);
		assert(itr != m_map_joints.end());
		for (int i = 0, n = itr->second.size(); i < n; ++i)
		{
			Joint* j = itr->second[i];
			for (auto& itr_child : j->m_children)
			{
				auto child = itr_child->m_spr;
				if (sprs.find(child) == sprs.end())
					buf.push(child);
			}
		}
	}

	for (auto& spr : sprs) {
		spr->Translate(offset);
	}
}

ee::SprPtr SkeletonData::GetSpriteByName(const std::vector<ee::SprPtr>& sprs, int name)
{
	for (int i = 0, n = sprs.size(); i < n; ++i)
		if (sprs[i]->GetName() == name)
			return sprs[i];
	return nullptr;
}

}