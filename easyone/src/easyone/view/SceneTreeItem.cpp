#include "view/SceneTreeItem.h"

namespace eone
{

SceneTreeItem::SceneTreeItem()
{
}

SceneTreeItem::SceneTreeItem(const SceneNodePtr& node)
	: m_node(node)
{
}

void SceneTreeItem::AddChild(SceneTreeItem* item)
{
	m_children.push_back(item);
}

}