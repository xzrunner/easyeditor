#pragma once

#include "ee0/Observer.h"
#include "ee0/SubjectMgr.h"

#include <ee/EditPanel.h>
#include <ee/SelectionSet.h>

#include <node0/SceneNode.h>

namespace ee0
{

class StagePage : public ee::EditPanel, public Observer
{
public:
	StagePage(wxWindow* parent, wxTopLevelWindow* frame);

	virtual void OnNotify(ee0::MessageID msg, const ee0::VariantSet& variants) override;

	ee0::SubjectMgr& GetSubjectMgr() { return m_sub_mgr; }

	const ee::SelectionSet<n0::SceneNode>& GetNodeSelection() const {
		return m_node_selection;
	}
	ee::SelectionSet<n0::SceneNode>& GetNodeSelection() {
		return m_node_selection;
	}

private:
	void NodeSelectionInsert(const ee0::VariantSet& variants);
	void NodeSelectionDelete(const ee0::VariantSet& variants);

protected:
	SubjectMgr m_sub_mgr;

	ee::SelectionSet<n0::SceneNode> m_node_selection;

}; // StagePage

}