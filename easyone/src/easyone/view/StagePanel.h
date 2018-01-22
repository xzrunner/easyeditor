#pragma once

#include "data/SceneNode.h"
#include "msg/SubjectMgr.h"
#include "msg/Observer.h"

#include <ee/EditPanel.h>

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

	//void InsertNode(const SceneNodePtr& node);
	//void DeleteNode(const SceneNodePtr& node);

	const std::vector<SceneNodePtr>& GetAllNodes() const { return m_nodes; }

	SubjectMgr& GetSubjectMgr() { return m_msg_mgr; }

private:
	void InsertSceneNode(const VariantSet& variants);
	void DeleteSceneNode(const VariantSet& variants);

private:
	std::vector<SceneNodePtr> m_nodes;

	SubjectMgr m_msg_mgr;

}; // StagePanel

}