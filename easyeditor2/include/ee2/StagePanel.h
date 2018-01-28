#pragma once

#include <ee/EditPanel.h>
#include <ee/SelectionSet.h>

#include <ee0/SubjectMgr.h>
#include <ee0/Observer.h>

#include <node0/SceneNode.h>

namespace ee { class LibraryPanel; }

namespace ee2
{

class StagePanel : public ee::EditPanel, public ee0::Observer
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
		ee::LibraryPanel* library);

	virtual void OnNotify(ee0::MessageID msg, const ee0::VariantSet& variants) override;

	const std::vector<n0::SceneNodePtr>& GetAllNodes() const { return m_nodes; }

	ee0::SubjectMgr& GetSubjectMgr() { return m_sub_mgr; }

private:
	void InsertSceneNode(const ee0::VariantSet& variants);
	void DeleteSceneNode(const ee0::VariantSet& variants);

private:
	std::vector<n0::SceneNodePtr> m_nodes;

	ee::SelectionSet<n0::SceneNode> m_node_selection;

	ee0::SubjectMgr m_sub_mgr;

}; // StagePanel

}