#include "SkeletonData.h"

#include "dataset/Joint.h"

namespace eanim
{
	SkeletonData::~SkeletonData()
	{
		clean();
	}

	void SkeletonData::copyFrom(const std::vector<d2d::ISprite*>& sprites,
		const SkeletonData& skeleton)
	{
		clean();

		// check
		std::map<d2d::ISprite*, std::vector<Joint*> >::const_iterator itr
			= skeleton.m_mapJoints.begin();
		for ( ; itr != skeleton.m_mapJoints.end(); ++itr)
			if (!getSpriteByName(sprites, itr->first->name))
				return;

		// prepare
		std::map<Joint*, Joint*> mapCovJoint;
		for (itr = skeleton.m_mapJoints.begin() ; itr != skeleton.m_mapJoints.end(); ++itr)
		{
			d2d::ISprite* sprite = getSpriteByName(sprites, itr->first->name);
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
		for (itr = skeleton.m_mapJoints.begin() ; itr != skeleton.m_mapJoints.end(); ++itr)
		{
			d2d::ISprite* src_sprite = itr->first;
			std::vector<Joint*> src_joints = itr->second;

			d2d::ISprite* dst_sprite = getSpriteByName(sprites, src_sprite->name);
			std::vector<Joint*> dst_joints;
			for (int i = 0, n = src_joints.size(); i < n; ++i)
			{
				std::map<Joint*, Joint*>::iterator find = mapCovJoint.find(src_joints[i]);
				assert(find != mapCovJoint.end());
				dst_joints.push_back(find->second);
			}

			m_mapJoints.insert(std::make_pair(dst_sprite, dst_joints));
		}
	}

	void SkeletonData::removeSprite(d2d::ISprite* sprite)
	{
		std::map<d2d::ISprite*, std::vector<Joint*> >::iterator itr
			= m_mapJoints.find(sprite);
		if (itr != m_mapJoints.end())
		{
			for (int i = 0, n = itr->second.size(); i < n; ++i)
				delete itr->second[i];
			m_mapJoints.erase(itr);
		}
	}

	void SkeletonData::insertJoint(d2d::ISprite* sprite, const d2d::Vector& pos)
	{
		std::map<d2d::ISprite*, std::vector<Joint*> >::iterator itr 
			= m_mapJoints.find(sprite);
		if (itr == m_mapJoints.end())
		{
			std::vector<Joint*> joints;
			joints.push_back(new Joint(sprite, pos));
			m_mapJoints.insert(std::make_pair(sprite, joints));
		}
		else
		{
			for (int i = 0, n = itr->second.size(); i < n; ++i)
				if (itr->second[i]->contain(pos))
					return;
			itr->second.push_back(new Joint(itr->first, pos));
		}
	}

	void SkeletonData::removeJoint(d2d::Vector& pos)
	{
		std::map<d2d::ISprite*, std::vector<Joint*> >::iterator itr 
			= m_mapJoints.begin();
		for ( ; itr != m_mapJoints.end(); ++itr)
		{
			std::vector<Joint*>::iterator itr_joint = itr->second.begin();
			for ( ; itr_joint != itr->second.end(); ++itr_joint)
			{
				if ((*itr_joint)->contain(pos))
				{
					delete *itr_joint;
					itr->second.erase(itr_joint);
					return;
				}
			}
		}
	}

	Joint* SkeletonData::queryJointByPos(const d2d::Vector& pos)
	{
		std::map<d2d::ISprite*, std::vector<Joint*> >::iterator itr 
			= m_mapJoints.begin();
		for ( ; itr != m_mapJoints.end(); ++itr)
		{
			std::vector<Joint*>::iterator itr_joint = itr->second.begin();
			for ( ; itr_joint != itr->second.end(); ++itr_joint)
				if ((*itr_joint)->contain(pos))
					return *itr_joint;
		}
		return NULL;
	}

	void SkeletonData::draw() const
	{
		std::map<d2d::ISprite*, std::vector<Joint*> >::const_iterator itr
			= m_mapJoints.begin();
		for ( ; itr != m_mapJoints.end(); ++itr)
			for (int i = 0, n = itr->second.size(); i < n; ++i)
				itr->second[i]->draw();
	}

	void SkeletonData::absorb(d2d::ISprite* sprite)
	{
		std::map<d2d::ISprite*, std::vector<Joint*> >::const_iterator itr_child 
			= m_mapJoints.find(sprite);
		if (itr_child == m_mapJoints.end())
			return;

		std::map<d2d::ISprite*, std::vector<Joint*> >::const_iterator itr_parent
			= m_mapJoints.begin();
		for ( ; itr_parent != m_mapJoints.end(); ++itr_parent)
		{
			if (itr_child->first == itr_parent->first)
				continue;
			for (int i = 0, n = itr_child->second.size(); i < n; ++i)
			{
				Joint* c = itr_child->second[i];
				d2d::Vector cp = c->getWorldPos();
				for (int j = 0, m = itr_parent->second.size(); j < m; ++j)
				{					
					Joint* p = itr_parent->second[j];
					if (p->intersect(cp))
					{
						d2d::Vector pp = p->getWorldPos();
						c->translate(pp - cp);
						p->connect(c);
						return;
					}
				}
				c->deconnect();
			}
		}
	}

	void SkeletonData::fixJoint(d2d::ISprite* sprite)
	{
		std::map<d2d::ISprite*, std::vector<Joint*> >::iterator itr 
			= m_mapJoints.find(sprite);
		if (itr == m_mapJoints.end()) return;

		for (int i = 0, n = itr->second.size(); i < n; ++i)
		{
			Joint* j = itr->second[i];
			if (j->m_parent)
			{
				sprite->translate(j->m_parent->getWorldPos() - j->getWorldPos());
				break;
			}
		}
	}

	void SkeletonData::updateJoint(d2d::ISprite* sprite, float dAngle)
	{
 		std::map<d2d::ISprite*, std::vector<Joint*> >::iterator itr 
 			= m_mapJoints.find(sprite);
 		if (itr == m_mapJoints.end()) return;

		for (int i = 0, n = itr->second.size(); i < n; ++i)
		{
 			Joint* p = itr->second[i];
			d2d::Vector pp = p->getWorldPos();
 			std::set<Joint*>::iterator itr_joint = p->m_children.begin();
 			for ( ; itr_joint != p->m_children.end(); ++itr_joint)
			{
				Joint* c = *itr_joint;
				d2d::Vector cp = c->getWorldPos();
				c->m_sprite->translate(pp - cp);
				if (dAngle != 0)
					c->m_sprite->rotate(dAngle);
				updateJoint(c->m_sprite, dAngle);
			}
		}
	}

	void SkeletonData::clean()
	{
		std::map<d2d::ISprite*, std::vector<Joint*> >::iterator itr
			= m_mapJoints.begin();
		for ( ; itr != m_mapJoints.end(); ++itr)
			for (int i = 0, n = itr->second.size(); i < n; ++i)
				delete itr->second[i];
		m_mapJoints.clear();
	}

	d2d::ISprite* SkeletonData::getSpriteByName(const std::vector<d2d::ISprite*>& sprites, const std::string& name)
	{
		for (int i = 0, n = sprites.size(); i < n; ++i)
			if (sprites[i]->name == name)
				return sprites[i];
		return NULL;
	}
}