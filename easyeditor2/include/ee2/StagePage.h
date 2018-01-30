#pragma once

#include <ee0/StagePage.h>

namespace ee { class LibraryPanel; }

namespace ee2
{

class StagePage : public ee0::StagePage
{
public:
	StagePage(wxWindow* parent, wxTopLevelWindow* frame,
		ee::LibraryPanel* library);

	virtual void OnNotify(ee0::MessageID msg, const ee0::VariantSet& variants) override;

	const std::vector<n0::SceneNodePtr>& GetAllNodes() const { return m_nodes; }

private:
	void InsertSceneNode(const ee0::VariantSet& variants);
	void DeleteSceneNode(const ee0::VariantSet& variants);

private:
	std::vector<n0::SceneNodePtr> m_nodes;

}; // StagePage

}