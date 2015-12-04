#include "SkeletonData.h"

#include "dataset/Joint.h"

#include <queue>

namespace eanim
{

SkeletonData::~SkeletonData()
{
	Clean();
}

void SkeletonData::CopyFrom(const SkeletonData& skeleton)
{
	std::vector<d2d::ISprite*> sprites;
	sprites.reserve(skeleton.m_map_joints.size());
	std::map<d2d::ISprite*, std::vector<Joint*> >::const_iterator itr 
		= skeleton.m_map_joints.begin();
	for ( ; itr != skeleton.m_map_joints.end(); ++itr)
		sprites.push_back(itr->first->Clone());
	CopyFrom(sprites, skeleton);
}

void SkeletonData::CopyFrom(const std::vector<d2d::ISprite*>& sprites,
	const SkeletonData& skeleton)
{
	Clean();

	// check
	std::map<d2d::ISprite*, std::vector<Joint*> >::const_iterator itr
		= skeleton.m_map_joints.begin();
	for ( ; itr != skeleton.m_map_joints.end(); ++itr)
		if (!GetSpriteByName(sprites, itr->first->name))
			return;

	// prepare
	std::map<Joint*, Joint*> mapCovJoint;
	for (itr = skeleton.m_map_joints.begin() ; itr != skeleton.m_map_joints.end(); ++itr)
	{
		d2d::ISprite* sprite = GetSpriteByName(sprites, itr->first->name);
		if (sprite == NULL)
			return;

		for (int i = 0, n = itr->second.size(); i < n; ++i)
		{
			Joint* src = itr->second[i];
			if (mapCovJoint.find(src) != mapCovJoint.end())
				continue;

			Joint* dst = new Joint(sprite);
			dst->m_relative = src->m_relative;
			dst->m_relativeAngle = src->m_relativeAngle;
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
		d2d::ISprite* src_sprite = itr->first;
		std::vector<Joint*> src_joints = itr->second;

		d2d::ISprite* dst_sprite = GetSpriteByName(sprites, src_sprite->name);
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

void SkeletonData::RemoveSprite(d2d::ISprite* sprite)
{
	std::map<d2d::ISprite*, std::vector<Joint*> >::iterator itr
		= m_map_joints.find(sprite);
	if (itr != m_map_joints.end())
	{
		for (int i = 0, n = itr->second.size(); i < n; ++i)
			delete itr->second[i];
		m_map_joints.erase(itr);
	}
}

bool SkeletonData::IsContainSprite(d2d::ISprite* sprite) const
{
	return m_map_joints.find(sprite) != m_map_joints.end();
}

void SkeletonData::InsertJoint(d2d::ISprite* sprite, const d2d::Vector& pos)
{
	std::map<d2d::ISprite*, std::vector<Joint*> >::iterator itr 
		= m_map_joints.find(sprite);
	if (itr == m_map_joints.end())
	{
		std::vector<Joint*> joints;
		joints.push_back(new Joint(sprite, pos));
		m_map_joints.insert(std::make_pair(sprite, joints));
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

void SkeletonData::RemoveJoint(d2d::Vector& pos)
{
	std::map<d2d::ISprite*, std::vector<Joint*> >::iterator itr 
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

Joint* SkeletonData::QueryJointByPos(const d2d::Vector& pos)
{
	std::map<d2d::ISprite*, std::vector<Joint*> >::iterator itr 
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
	std::map<d2d::ISprite*, std::vector<Joint*> >::const_iterator itr
		= m_map_joints.begin();
	for ( ; itr != m_map_joints.end(); ++itr) {
		for (int i = 0, n = itr->second.size(); i < n; ++i) {
			itr->second[i]->Draw();
		}
	}
}

void SkeletonData::Absorb(d2d::ISprite* sprite)
{
	std::map<d2d::ISprite*, std::vector<Joint*> >::const_iterator itr_child 
		= m_map_joints.find(sprite);
	if (itr_child == m_map_joints.end())
		return;

	std::map<d2d::ISprite*, std::vector<Joint*> >::const_iterator itr_parent
		= m_map_joints.begin();
	for ( ; itr_parent != m_map_joints.end(); ++itr_parent)
	{
		if (itr_child->first == itr_parent->first)
			continue;
		for (int i = 0, n = itr_child->second.size(); i < n; ++i)
		{
			Joint* c = itr_child->second[i];
			d2d::Vector cp = c->GetWorldPos();
			for (int j = 0, m = itr_parent->second.size(); j < m; ++j)
			{					
				Joint* p = itr_parent->second[j];
				if (p->Intersect(cp))
				{
					d2d::Vector pp = p->GetWorldPos();
					bool success = p->Connect(c);
					if (success) {
						Translate(c->m_sprite, pp - cp);
						return;
					}
				}
			}
			c->Deconnect();
		}
	}
}

void SkeletonData::FixJoint(d2d::ISprite* sprite)
{
	std::map<d2d::ISprite*, std::vector<Joint*> >::iterator itr 
		= m_map_joints.find(sprite);
	if (itr == m_map_joints.end()) return;

	for (int i = 0, n = itr->second.size(); i < n; ++i)
	{
		Joint* j = itr->second[i];
		if (j->m_parent)
		{
			sprite->Translate(j->m_parent->GetWorldPos() - j->GetWorldPos());
			break;
		}
	}
}

void SkeletonData::UpdateJoint(d2d::ISprite* sprite, float dAngle)
{
	std::map<d2d::ISprite*, std::vector<Joint*> >::iterator itr 
		= m_map_joints.find(sprite);
	if (itr == m_map_joints.end()) return;

	for (int i = 0, n = itr->second.size(); i < n; ++i)
	{
		Joint* p = itr->second[i];
		d2d::Vector pp = p->GetWorldPos();
		std::set<Joint*>::iterator itr_joint = p->m_children.begin();
		for ( ; itr_joint != p->m_children.end(); ++itr_joint)
		{
			Joint* c = *itr_joint;
			d2d::Vector cp = c->GetWorldPos();
			c->m_sprite->Translate(pp - cp);
			if (dAngle != 0)
				c->m_sprite->Rotate(dAngle);
			UpdateJoint(c->m_sprite, dAngle);
		}
	}
}

void SkeletonData::GetTweenSprites(SkeletonData& start, SkeletonData& end, 
	std::vector<d2d::ISprite*>& tween, float process)
{
	SkeletonData mid;
	mid.CopyFrom(start);

	std::map<d2d::ISprite*, std::vector<Joint*> >::const_iterator itr_s
		= start.m_map_joints.begin();
	for ( ; itr_s != start.m_map_joints.end(); ++itr_s)
	{
		d2d::ISprite* s = itr_s->first;
		std::map<d2d::ISprite*, std::vector<Joint*> >::const_iterator itr_e
			= end.m_map_joints.begin();
		for ( ; itr_e != end.m_map_joints.end(); ++itr_e)
		{
			if (s->name == itr_e->first->name)
			{
				d2d::ISprite* e = itr_e->first;
				
				d2d::ISprite* sprite = NULL;
				std::map<d2d::ISprite*, std::vector<Joint*> >::iterator itr_mid = mid.m_map_joints.begin();
				for ( ; itr_mid != mid.m_map_joints.end(); ++itr_mid)
				{
					if (itr_mid->first->name == s->name)
					{
						sprite = itr_mid->first;
						break;
					}
				}
				assert(sprite);

				float delta = (e->GetAngle() - s->GetAngle()) * process;
				sprite->Rotate(delta);
				mid.FixJoint(sprite);

				break;
			}
		}
	}

	std::map<d2d::ISprite*, std::vector<Joint*> >::iterator itr = mid.m_map_joints.begin();
	for ( ; itr != mid.m_map_joints.end(); ++itr)
		tween.push_back(itr->first);
}

void SkeletonData::Clean()
{
	std::map<d2d::ISprite*, std::vector<Joint*> >::iterator itr
		= m_map_joints.begin();
	for ( ; itr != m_map_joints.end(); ++itr)
		for (int i = 0, n = itr->second.size(); i < n; ++i)
			delete itr->second[i];
	m_map_joints.clear();
}

void SkeletonData::Translate(d2d::ISprite* sprite, const d2d::Vector& offset)
{
	std::set<d2d::ISprite*> sprites;
	std::queue<d2d::ISprite*> buf;
	buf.push(sprite);
	while (!buf.empty())
	{
		d2d::ISprite* curr = buf.front(); buf.pop();
		sprites.insert(curr);

		std::map<d2d::ISprite*, std::vector<Joint*> >::iterator itr 
			= m_map_joints.find(curr);
		assert(itr != m_map_joints.end());
		for (int i = 0, n = itr->second.size(); i < n; ++i)
		{
			Joint* j = itr->second[i];

			std::set<Joint*>::iterator itr_child = j->m_children.begin();
			for ( ; itr_child != j->m_children.end(); ++itr_child)
			{
				d2d::ISprite* child = (*itr_child)->m_sprite;
				if (sprites.find(child) == sprites.end())
					buf.push(child);
			}
		}
	}

	std::set<d2d::ISprite*>::iterator itr = sprites.begin();
	for ( ; itr != sprites.end(); ++itr)
		(*itr)->Translate(offset);
}

d2d::ISprite* SkeletonData::GetSpriteByName(const std::vector<d2d::ISprite*>& sprites, const std::string& name)
{
	for (int i = 0, n = sprites.size(); i < n; ++i)
		if (sprites[i]->name == name)
			return sprites[i];
	return NULL;
}

}