#include "SceneNode.h"

#include <stddef.h>

namespace ee
{

SceneNodeMgr* SceneNodeMgr::m_instance = NULL;

SceneNodeMgr* SceneNodeMgr::Instance()
{
	if (!m_instance) {
		m_instance = new SceneNodeMgr;
	}
	return m_instance;
}

void SceneNodeMgr::Add(SceneNode* node)
{
	m_nodes.insert(node);
}

void SceneNodeMgr::Remove(SceneNode* node)
{
	m_nodes.erase(node);
}

bool SceneNodeMgr::Update(float dt)
{
	bool dirty = false;
	std::set<SceneNode*>::iterator itr = m_nodes.begin();
	for ( ; itr != m_nodes.end(); ++itr) {
		if ((*itr)->Update(dt)) {
			dirty = true;
		}
	}
	return dirty;
}

void SceneNodeMgr::Draw() const
{
	std::set<SceneNode*>::const_iterator itr = m_nodes.begin();
	for ( ; itr != m_nodes.end(); ++itr) {
		(*itr)->Draw();
	}
}

}