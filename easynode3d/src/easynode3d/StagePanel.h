#pragma once

#include "NodeSelection.h"

#include <ee/EditPanel.h>
#include <ee/MultiSpritesImpl.h>

#include <node3/INode.h>

namespace ee { class LibraryPanel; }

namespace enode3d
{

class StagePanel : public ee::EditPanel
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
		ee::LibraryPanel* library);

	sm::vec3 TransPosScrToProj(int x, int y) const;

	void InsertNode(const n3::NodePtr& node);
	void DeleteNode(const n3::NodePtr& node);

	const std::vector<n3::NodePtr>& GetAllNodes() const { return m_nodes; }

	auto& GetNodeSelection() { return m_selection; }
	
private:
	std::vector<n3::NodePtr> m_nodes;

	NodeSelection m_selection;

}; // StagePanel

}