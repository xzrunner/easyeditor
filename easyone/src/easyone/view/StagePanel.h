#pragma once

#include "data/SceneNode.h"
#include "msg/Subject.h"

#include <ee/EditPanel.h>

namespace ee { class LibraryPanel; }

namespace eone
{

class StagePanel : public ee::EditPanel
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
		ee::LibraryPanel* library);

	sm::vec3 TransPosScrToProj3d(int x, int y) const;

	void InsertNode(const SceneNodePtr& node);
	void DeleteNode(const SceneNodePtr& node);

	const std::vector<SceneNodePtr>& GetAllNodes() const { return m_nodes; }

public:
	struct Subjects
	{
		Subject sub_insert_node;
	};

	Subjects& GetSubjects() { return m_subjects; }

private:
	std::vector<SceneNodePtr> m_nodes;

	Subjects m_subjects;

}; // StagePanel

}