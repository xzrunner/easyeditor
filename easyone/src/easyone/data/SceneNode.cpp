#include "data/SceneNode.h"

namespace eone
{

SceneNode::SceneNode()
{
	static size_t COUNT = 0;
	m_name = "node" + std::to_string(COUNT++);
}

const std::string& SceneNode::GetName() const
{
	return m_name;
}

void SceneNode::SetName(const std::string& name)
{
	m_name = name;
}

}