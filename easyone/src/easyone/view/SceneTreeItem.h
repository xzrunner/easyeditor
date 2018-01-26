#pragma once

#include <node3/SceneNode.h>

#include <wx/treectrl.h>

namespace eone
{

class SceneTreeItem : public wxTreeItemData
{
public:
	SceneTreeItem();
	SceneTreeItem(const n3::SceneNodePtr& node);

	void AddChild(SceneTreeItem* item);

	size_t GetChildrenNum() const { return m_children.size(); }

	const n3::SceneNodePtr& GetNode() const { return m_node; }

private:
	n3::SceneNodePtr m_node = nullptr;

	std::vector<SceneTreeItem*> m_children;

}; // SceneTreeItem

}