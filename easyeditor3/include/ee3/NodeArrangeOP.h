#pragma once

#include "ee3/NodeSelectOP.h"

#include <ee0/SelectionSet.h>
#include <ee0/typedef.h>

namespace ee0 { class EditOpState; class SubjectMgr; }

namespace ee3
{

class WxStageCanvas;

class NodeArrangeOP : public NodeSelectOP
{
public:
	NodeArrangeOP(WxStagePage& stage);

	virtual bool OnKeyDown(int keyCode) override;
	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseLeftUp(int x, int y) override;
	virtual bool OnMouseRightDown(int x, int y) override;
	virtual bool OnMouseRightUp(int x, int y) override;
	virtual bool OnMouseMove(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;
	virtual bool OnMouseWheelRotation(int x, int y, int direction) override;

private:
	void ChangeEditOpState(const ee0::EditOpStatePtr& state);

private:
	ee0::SubjectMgr& m_sub_mgr;

	ee0::SelectionSet<n0::SceneNode>& m_node_selection;

	std::shared_ptr<WxStageCanvas> m_canvas = nullptr;

	ee0::EditOpStatePtr m_op_state = nullptr;

	ee0::EditOpStatePtr m_cam_rotate_state    = nullptr;
	ee0::EditOpStatePtr m_cam_translate_state = nullptr;
	ee0::EditOpStatePtr m_cam_zoom_state      = nullptr;

	ee0::EditOpStatePtr m_node_rotate_state    = nullptr;
	ee0::EditOpStatePtr m_node_translate_state = nullptr;

}; // NodeArrangeOP

}
