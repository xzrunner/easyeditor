#include "SkeletonData.h"

#include "dataset/Joint.h"

namespace eanim
{
	SkeletonData::~SkeletonData()
	{
		std::map<d2d::ISprite*, std::vector<Joint*> >::iterator itr
			= m_mapJoints.begin();
		for ( ; itr != m_mapJoints.end(); ++itr)
			for (int i = 0, n = itr->second.size(); i < n; ++i)
				delete itr->second[i];
		m_mapJoints.clear();
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
}