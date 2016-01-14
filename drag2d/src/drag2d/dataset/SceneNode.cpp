#include "SceneNode.h"

#include <stddef.h>

namespace d2d
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

void SceneNodeMgr::Update(float dt)
{
	std::set<SceneNode*>::iterator itr = m_nodes.begin();
	for ( ; itr != m_nodes.end(); ++itr) {
		(*itr)->Update(dt);
	}
}

void SceneNodeMgr::Draw() const
{
	std::set<SceneNode*>::const_iterator itr = m_nodes.begin();
	for ( ; itr != m_nodes.end(); ++itr) {
		(*itr)->Draw();
	}
}

}