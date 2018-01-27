#pragma once

#include "ee3/NodeSelectOP.h"

#include <ee/SelectionSet.h>

#include <ee0/EditOpState.h>

#include <memory>

namespace ee0 { class EditOpState; class SubjectMgr; }

namespace ee3
{

class StageCanvas;

class NodeArrangeOP : public NodeSelectOP
{
public:
	NodeArrangeOP(StagePanel& stage);

	virtual bool OnKeyDown(int keyCode) override;
	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseLeftUp(int x, int y) override;
	virtual bool OnMouseRightDown(int x, int y) override;
	virtual bool OnMouseRightUp(int x, int y) override;
	virtual bool OnMouseMove(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;
	virtual bool OnMouseWheelRotation(int x, int y, int direction) override;

private:
	ee0::SubjectMgr& m_sub_mgr;

	ee::SelectionSet<n3::SceneNode>& m_node_selection;

	std::shared_ptr<StageCanvas> m_canvas = nullptr;

	std::unique_ptr<ee0::EditOpState> m_op_state = nullptr;

}; // NodeArrangeOP

}
