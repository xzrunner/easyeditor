#pragma once

#include "data/SceneNode.h"

#include <wx/treectrl.h>

namespace eone
{

class SceneTreeItem : public wxTreeItemData
{
public:
	virtual ~SceneTreeItem() {}
	virtual bool IsLeaf() const = 0;
}; // SceneTreeItem

class SceneTreeItemLeaf : public SceneTreeItem
{
public:
	SceneTreeItemLeaf(const SceneNodePtr& node)
		: m_node(node) 
	{}

	virtual bool IsLeaf() const override { return true; }

	const SceneNodePtr& GetNode() const { return m_node; }

private:
	SceneNodePtr m_node = nullptr;

}; // SceneTreeItemLeaf

class SceneTreeItemIndex : public SceneTreeItem
{
public:
	virtual bool IsLeaf() const override { return false; }

private:
	std::vector<SceneTreeItem> m_children;
	
}; // SceneTreeItemIndex

}