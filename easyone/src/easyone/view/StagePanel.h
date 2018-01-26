#pragma once

#include "msg/SubjectMgr.h"
#include "msg/Observer.h"

#include <ee/EditPanel.h>
#include <ee/SelectionSet.h>

#include <node3/SceneNode.h>

namespace ee { class LibraryPanel; }

namespace eone
{

class StagePanel : public ee::EditPanel, public Observer
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
		ee::LibraryPanel* library);

	virtual void OnNotify(MessageID msg, const VariantSet& variants) override;

	sm::vec3 TransPosScrToProj3d(int x, int y) const;

	const std::vector<n3::SceneNodePtr>& GetAllNodes() const { return m_nodes; }

	const ee::SelectionSet<n3::SceneNode>& GetNodeSelection() const {
		return m_node_selection; 
	}
	ee::SelectionSet<n3::SceneNode>& GetNodeSelection() {
		return m_node_selection;
	}

	SubjectMgr& GetSubjectMgr() { return m_sub_mgr; }

private:
	void InsertSceneNode(const VariantSet& variants);
	void DeleteSceneNode(const VariantSet& variants);

	void NodeSelectionInsert(const VariantSet& variants);
	void NodeSelectionDelete(const VariantSet& variants);

private:
	std::vector<n3::SceneNodePtr> m_nodes;

	ee::SelectionSet<n3::SceneNode> m_node_selection;
	
	SubjectMgr m_sub_mgr;

}; // StagePanel

}