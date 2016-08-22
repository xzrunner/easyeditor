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
	std::vector<ee::Sprite*> sprs;
	sprs.reserve(skeleton.m_map_joints.size());
	std::map<ee::Sprite*, std::vector<Joint*> >::const_iterator itr 
		= skeleton.m_map_joints.begin();
	for ( ; itr != skeleton.m_map_joints.end(); ++itr) {
		ee::Sprite* spr = dynamic_cast<ee::Sprite*>(((cu::Cloneable*)itr->first)->Clone());
		sprs.push_back(spr);
	}
	CopyFrom(sprs, skeleton);
}

void SkeletonData::CopyFrom(const std::vector<ee::Sprite*>& sprs,
	const SkeletonData& skeleton)
{
	Clean();

	// check
	std::map<ee::Sprite*, std::vector<Joint*> >::const_iterator itr
		= skeleton.m_map_joints.begin();
	for ( ; itr != skeleton.m_map_joints.end(); ++itr)
		if (!GetSpriteByName(sprs, itr->first->GetName()))
			return;

	// prepare
	std::map<Joint*, Joint*> mapCovJoint;
	for (itr = skeleton.m_map_joints.begin() ; itr != skeleton.m_map_joints.end(); ++itr)
	{
		ee::Sprite* spr = GetSpriteByName(sprs, itr->first->GetName());
		if (spr == NULL)
			return;

		for (int i = 0, n = itr->second.size(); i < n; ++i)
		{
			Joint* src = itr->second[i];
			if (mapCovJoint.find(src) != mapCovJoint.end())
				continue;

			Joint* dst = new Joint(spr);
			dst->m_relative_pos = src->m_relative_pos;
			dst->m_relative_angle = src->m_relative_angle;
			mapCovJoint.insert(std::make_pair(src, dst));
		}
	}

	// convert joint
	std::map<Joint*, Joint*>::iterator itr_joint = mapCovJoint.begin();
	for ( ; itr_joint != mapCovJoint.end(); ++itr_joint)
	{
		Joint* src = itr_joint->first;
		Joint* dst = itr_joint->second;

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
	for (itr = skeleton.m_map_joints.begin() ; itr != skeleton.m_map_joints.end(); ++itr)
	{
		ee::Sprite* src_sprite = itr->first;
		std::vector<Joint*> src_joints = itr->second;

		ee::Sprite* dst_sprite = GetSpriteByName(sprs, src_sprite->GetName());
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

void SkeletonData::RemoveSprite(ee::Sprite* spr)
{
	std::map<ee::Sprite*, std::vector<Joint*> >::iterator itr
		= m_map_joints.find(spr);
	if (itr != m_map_joints.end())
	{
		for (int i = 0, n = itr->second.size(); i < n; ++i)
			delete itr->second[i];
		m_map_joints.erase(itr);
	}
}

bool SkeletonData::IsContainSprite(ee::Sprite* spr) const
{
	return m_map_joints.find(spr) != m_map_joints.end();
}

void SkeletonData::InsertJoint(ee::Sprite* spr, const sm::vec2& pos)
{
	std::map<ee::Sprite*, std::vector<Joint*> >::iterator itr 
		= m_map_joints.find(spr);
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
	std::map<ee::Sprite*, std::vector<Joint*> >::iterator itr 
		= m_map_joints.begin();
	for ( ; itr != m_map_joints.end(); ++itr)
	{
		std::vector<Joint*>::iterator itr_joint = itr->second.begin();
		for ( ; itr_joint != itr->second.end(); ++itr_joint)
		{
			Joint* joint = *itr_joint;
			if (joint->Contain(pos))
			{
				delete joint;
				itr->second.erase(itr_joint);
				return;
			}
		}
	}
}

Joint* SkeletonData::QueryJointByPos(const sm::vec2& pos)
{
	std::map<ee::Sprite*, std::vector<Joint*> >::iterator itr 
		= m_map_joints.begin();
	for ( ; itr != m_map_joints.end(); ++itr)
	{
		std::vector<Joint*>::iterator itr_joint = itr->second.begin();
		for ( ; itr_joint != itr->second.end(); ++itr_joint) {
			if ((*itr_joint)->Contain(pos)) {
				return *itr_joint;
			}
		}
	}
	return NULL;
}

void SkeletonData::Draw() const
{
	std::map<ee::Sprite*, std::vector<Joint*> >::const_iterator itr
		= m_map_joints.begin();
	for ( ; itr != m_map_joints.end(); ++itr) {
		for (int i = 0, n = itr->second.size(); i < n; ++i) {
			itr->second[i]->Draw();
		}
	}
}

void SkeletonData::Absorb(ee::Sprite* spr)
{
	std::map<ee::Sprite*, std::vector<Joint*> >::const_iterator itr_child 
		= m_map_joints.find(spr);
	if (itr_child == m_map_joints.end())
		return;

	std::map<ee::Sprite*, std::vector<Joint*> >::const_iterator itr_parent
		= m_map_joints.begin();
	for ( ; itr_parent != m_map_joints.end(); ++itr_parent)
	{
		if (itr_child->first == itr_parent->first)
			continue;
		for (int i = 0, n = itr_child->second.size(); i < n; ++i)
		{
			Joint* c = itr_child->second[i];
			sm::vec2 cp = c->GetWorldPos();
			for (int j = 0, m = itr_parent->second.size(); j < m; ++j)
			{					
				Joint* p = itr_parent->second[j];
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

void SkeletonData::FixJoint(ee::Sprite* spr)
{
	std::map<ee::Sprite*, std::vector<Joint*> >::iterator itr 
		= m_map_joints.find(spr);
	if (itr == m_map_joints.end()) return;

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

void SkeletonData::UpdateJoint(ee::Sprite* spr, float dAngle)
{
	std::map<ee::Sprite*, std::vector<Joint*> >::iterator itr 
		= m_map_joints.find(spr);
	if (itr == m_map_joints.end()) return;

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
								   std::vector<ee::Sprite*>& tween, float process)
{
	SkeletonData mid;
	mid.CopyFrom(start);

	std::map<ee::Sprite*, std::vector<Joint*> >::const_iterator itr_s
		= start.m_map_joints.begin();
	for ( ; itr_s != start.m_map_joints.end(); ++itr_s)
	{
		ee::Sprite* s = itr_s->first;
		std::map<ee::Sprite*, std::vector<Joint*> >::const_iterator itr_e
			= end.m_map_joints.begin();
		for ( ; itr_e != end.m_map_joints.end(); ++itr_e)
		{
			if (s->GetName() == itr_e->first->GetName())
			{
				ee::Sprite* e = itr_e->first;
				
				ee::Sprite* spr = NULL;
				std::map<ee::Sprite*, std::vector<Joint*> >::iterator itr_mid = mid.m_map_joints.begin();
				for ( ; itr_mid != mid.m_map_joints.end(); ++itr_mid)
				{
					if (itr_mid->first->GetName() == s->GetName())
					{
						spr = itr_mid->first;
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

	std::map<ee::Sprite*, std::vector<Joint*> >::iterator itr = mid.m_map_joints.begin();
	for ( ; itr != mid.m_map_joints.end(); ++itr)
		tween.push_back(itr->first);
}

void SkeletonData::Clean()
{
	std::map<ee::Sprite*, std::vector<Joint*> >::iterator itr
		= m_map_joints.begin();
	for ( ; itr != m_map_joints.end(); ++itr)
		for (int i = 0, n = itr->second.size(); i < n; ++i)
			delete itr->second[i];
	m_map_joints.clear();
}

void SkeletonData::Translate(ee::Sprite* spr, const sm::vec2& offset)
{
	std::set<ee::Sprite*> sprs;
	std::queue<ee::Sprite*> buf;
	buf.push(spr);
	while (!buf.empty())
	{
		ee::Sprite* curr = buf.front(); buf.pop();
		sprs.insert(curr);

		std::map<ee::Sprite*, std::vector<Joint*> >::iterator itr 
			= m_map_joints.find(curr);
		assert(itr != m_map_joints.end());
		for (int i = 0, n = itr->second.size(); i < n; ++i)
		{
			Joint* j = itr->second[i];

			std::set<Joint*>::iterator itr_child = j->m_children.begin();
			for ( ; itr_child != j->m_children.end(); ++itr_child)
			{
				ee::Sprite* child = (*itr_child)->m_spr;
				if (sprs.find(child) == sprs.end())
					buf.push(child);
			}
		}
	}

	std::set<ee::Sprite*>::iterator itr = sprs.begin();
	for ( ; itr != sprs.end(); ++itr)
		(*itr)->Translate(offset);
}

ee::Sprite* SkeletonData::GetSpriteByName(const std::vector<ee::Sprite*>& sprs, const std::string& name)
{
	for (int i = 0, n = sprs.size(); i < n; ++i)
		if (sprs[i]->GetName() == name)
			return sprs[i];
	return NULL;
}

}