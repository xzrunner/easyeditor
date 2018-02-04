#include "ee3/NodeArrangeOP.h"
#include "ee3/WxStagePage.h"
#include "ee3/WxStageCanvas.h"
#include "ee3/CamTranslateState.h"
#include "ee3/CamRotateState.h"
#include "ee3/CamZoomState.h"
#include "ee3/NodeTranslateState.h"
#include "ee3/NodeRotateState.h"

#include <node0/SceneNode.h>
#include <node3/CompTransform.h>

namespace ee3
{

NodeArrangeOP::NodeArrangeOP(WxStagePage& stage)
	: NodeSelectOP(stage)
	, m_sub_mgr(stage.GetSubjectMgr())
	, m_node_selection(stage.GetNodeSelection())
	, m_canvas(std::dynamic_pointer_cast<WxStageCanvas>(stage.GetImpl().GetCanvas()))
{
	auto& cam = m_canvas->GetCamera();
	auto& vp = m_canvas->GetViewport();

	m_cam_rotate_state    = std::make_shared<CamRotateState>(cam, m_sub_mgr);
	m_cam_translate_state = std::make_shared<CamTranslateState>(cam, m_sub_mgr);
	m_cam_zoom_state      = std::make_shared<CamZoomState>(cam, vp, m_sub_mgr);

	m_node_rotate_state    = std::make_shared<NodeRotateState>(cam, vp, m_sub_mgr, m_node_selection);
	m_node_translate_state = std::make_shared<NodeTranslateState>(cam, vp, m_sub_mgr, m_node_selection);

	m_op_state = m_cam_rotate_state;
}

bool NodeArrangeOP::OnKeyDown(int keyCode)
{
	if (NodeSelectOP::OnKeyDown(keyCode)) {
		return true;
	}

	switch (keyCode)
	{
	case WXK_ESCAPE:
		m_canvas->GetCamera().Reset();
		break;
	case WXK_SPACE:
		{
			m_node_selection.Traverse(
				[](const n0::SceneNodePtr& node)->bool
				{
					auto& ctrans = node->GetComponent<n3::CompTransform>();
					ctrans.SetPosition(sm::vec3(0, 0, 0));
					ctrans.SetAngle(sm::Quaternion());
					ctrans.SetScale(sm::vec3(1, 1, 1));
					return true;
				}
			);
			m_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
		}
		break;
	}

	return false;
}

bool NodeArrangeOP::OnMouseLeftDown(int x, int y)
{
	if (NodeSelectOP::OnMouseLeftDown(x, y)) {
		return true;
	}

	auto& selection = m_stage.GetNodeSelection();
	if (selection.IsEmpty()) {
		ChangeEditOpState(m_cam_rotate_state);
	} else {
		ChangeEditOpState(m_node_translate_state);
	}

	return m_op_state->OnMousePress(x, y);
}

bool NodeArrangeOP::OnMouseLeftUp(int x, int y)
{
	if (NodeSelectOP::OnMouseLeftUp(x, y)) {
		return true;
	}

	m_op_state->OnMouseRelease(x, y);

	ChangeEditOpState(m_cam_zoom_state);

	return false;
}

bool NodeArrangeOP::OnMouseRightDown(int x, int y)
{
	if (NodeSelectOP::OnMouseRightDown(x, y)) {
		return true;
	}

	auto& selection = m_stage.GetNodeSelection();
	if (selection.IsEmpty()) {
		ChangeEditOpState(m_cam_translate_state);
	} else if (selection.Size() == 1) {
		ChangeEditOpState(m_node_rotate_state);
	}

	return m_op_state->OnMousePress(x, y);
}

bool NodeArrangeOP::OnMouseRightUp(int x, int y)
{
	if (NodeSelectOP::OnMouseRightUp(x, y)) {
		return true;
	}

	m_op_state->OnMouseRelease(x, y);

	ChangeEditOpState(m_cam_zoom_state);

	return false;
}

bool NodeArrangeOP::OnMouseMove(int x, int y)
{
	if (NodeSelectOP::OnMouseMove(x, y)) {
		return true;
	}

	//m_stage->SetFocus();

	return m_op_state->OnMouseMove(x, y);
}

bool NodeArrangeOP::OnMouseDrag(int x, int y)
{
	if (NodeSelectOP::OnMouseDrag(x, y)) {
		return true;
	}

	return m_op_state->OnMouseDrag(x, y);
}

bool NodeArrangeOP::OnMouseWheelRotation(int x, int y, int direction)
{
	if (NodeSelectOP::OnMouseWheelRotation(x, y, direction)) {
		return true;
	}

	return m_op_state->OnMouseWheelRotation(x, y, direction);
}

void NodeArrangeOP::ChangeEditOpState(const ee0::EditOpStatePtr& state)
{
	if (m_op_state == state) {
		return;
	}

	if (m_op_state) {
		m_op_state->UnBind();
	}
	m_op_state = state;
	if (m_op_state) {
		m_op_state->Bind();
	}
}

}