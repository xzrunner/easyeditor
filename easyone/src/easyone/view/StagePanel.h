#pragma once

#include "data/SceneNode.h"
#include "msg/SubjectMgr.h"
#include "msg/Observer.h"

#include <ee/EditPanel.h>
#include <ee/SelectionSet.h>

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

	const std::vector<SceneNodePtr>& GetAllNodes() const { return m_nodes; }

	const ee::SelectionSet<SceneNode>& GetNodeSelection() const { 
		return m_node_selection; 
	}

	SubjectMgr& GetSubjectMgr() { return m_sub_mgr; }

private:
	void InsertSceneNode(const VariantSet& variants);
	void DeleteSceneNode(const VariantSet& variants);

private:
	std::vector<SceneNodePtr> m_nodes;

	ee::SelectionSet<SceneNode> m_node_selection;
	
	SubjectMgr m_sub_mgr;

}; // StagePanel

}