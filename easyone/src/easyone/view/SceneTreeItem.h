#pragma once

#include "data/SceneNode.h"

#include <wx/treectrl.h>

namespace eone
{

class SceneTreeItem : public wxTreeItemData
{
public:
	SceneTreeItem();
	SceneTreeItem(const SceneNodePtr& node);

	void AddChild(SceneTreeItem* item);

	size_t GetChildrenNum() const { return m_children.size(); }

	const SceneNodePtr& GetNode() const { return m_node; }

private:
	SceneNodePtr m_node = nullptr;

	std::vector<SceneTreeItem*> m_children;

}; // SceneTreeItem

}