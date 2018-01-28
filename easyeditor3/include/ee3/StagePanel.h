#pragma once

#include <ee/EditPanel.h>
#include <ee/SelectionSet.h>

#include <ee0/SubjectMgr.h>
#include <ee0/Observer.h>

#include <node0/SceneNode.h>

namespace ee { class LibraryPanel; }

namespace ee3
{

class StagePanel : public ee::EditPanel, public ee0::Observer
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
		ee::LibraryPanel* library);

	virtual void OnNotify(ee0::MessageID msg, const ee0::VariantSet& variants) override;

	sm::vec3 TransPosScrToProj3d(int x, int y) const;

	const std::vector<n0::SceneNodePtr>& GetAllNodes() const { return m_nodes; }

	const ee::SelectionSet<n0::SceneNode>& GetNodeSelection() const {
		return m_node_selection; 
	}
	ee::SelectionSet<n0::SceneNode>& GetNodeSelection() {
		return m_node_selection;
	}

	ee0::SubjectMgr& GetSubjectMgr() { return m_sub_mgr; }

private:
	void InsertSceneNode(const ee0::VariantSet& variants);
	void DeleteSceneNode(const ee0::VariantSet& variants);

	void NodeSelectionInsert(const ee0::VariantSet& variants);
	void NodeSelectionDelete(const ee0::VariantSet& variants);

private:
	std::vector<n0::SceneNodePtr> m_nodes;

	ee::SelectionSet<n0::SceneNode> m_node_selection;
	
	ee0::SubjectMgr m_sub_mgr;

}; // StagePanel

}