#include "ee3/NodeArrangeOP.h"
#include "ee3/StagePanel.h"
#include "ee3/StageCanvas.h"
#include "ee3/CamTranslateState.h"
#include "ee3/CamRotateState.h"
#include "ee3/CamZoomState.h"
#include "ee3/NodeTranslateState.h"
#include "ee3/NodeRotateState.h"

#include <ee/FetchAllVisitor.h>

namespace ee3
{

NodeArrangeOP::NodeArrangeOP(StagePanel& stage)
	: NodeSelectOP(stage)
	, m_sub_mgr(stage.GetSubjectMgr())
	, m_node_selection(stage.GetNodeSelection())
	, m_canvas(std::dynamic_pointer_cast<StageCanvas>(stage.GetCanvas()))
{
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
				[](const n3::SceneNodePtr& node)->bool
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
		m_op_state = std::make_unique<CamRotateState>(
			m_canvas->GetCamera(), sm::vec2(x, y));
	} else {
		m_op_state = std::make_unique<NodeTranslateState>(
			m_canvas->GetCamera(), m_canvas->GetViewport(), 
			m_stage.GetSubjectMgr(), selection);
	}

	if (m_op_state) {
		m_op_state->OnMousePress(sm::vec2(x, y));
	}

	return false;
}

bool NodeArrangeOP::OnMouseLeftUp(int x, int y)
{
	if (NodeSelectOP::OnMouseLeftUp(x, y)) {
		return true;
	}

	if (m_op_state) {
		m_op_state->OnMouseRelease(sm::vec2(x, y));
	}

	m_op_state = std::make_unique<CamZoomState>(
		m_canvas->GetCamera(), m_canvas->GetViewport());

	return false;
}

bool NodeArrangeOP::OnMouseRightDown(int x, int y)
{
	if (NodeSelectOP::OnMouseRightDown(x, y)) {
		return true;
	}

	auto& selection = m_stage.GetNodeSelection();
	if (selection.IsEmpty()) {
		m_op_state = std::make_unique<CamTranslateState>(
			m_canvas->GetCamera(), sm::vec2(x, y));
	} else if (selection.Size() == 1) {
		m_op_state = std::make_unique<NodeRotateState>(
			m_canvas->GetCamera(), m_canvas->GetViewport(),
			m_stage.GetSubjectMgr(), selection);
	}

	if (m_op_state) {
		m_op_state->OnMousePress(sm::vec2(x, y));
	}

	return false;
}

bool NodeArrangeOP::OnMouseRightUp(int x, int y)
{
	if (NodeSelectOP::OnMouseRightUp(x, y)) {
		return true;
	}

	if (m_op_state) {
		m_op_state->OnMouseRelease(sm::vec2(x, y));
	}

	m_op_state = std::make_unique<CamZoomState>(
		m_canvas->GetCamera(), m_canvas->GetViewport());

	return false;
}

bool NodeArrangeOP::OnMouseMove(int x, int y)
{
	if (NodeSelectOP::OnMouseMove(x, y)) {
		return true;
	}

	//m_stage->SetFocus();

	if (m_op_state) {
		m_op_state->OnMouseMove(sm::vec2(x, y));
	}

	return false;
}

bool NodeArrangeOP::OnMouseDrag(int x, int y)
{
	if (NodeSelectOP::OnMouseDrag(x, y)) {
		return true;
	}

	if (m_op_state) {
		m_op_state->OnMouseDrag(sm::vec2(x, y));
	}

	return false;
}

bool NodeArrangeOP::OnMouseWheelRotation(int x, int y, int direction)
{
	if (NodeSelectOP::OnMouseWheelRotation(x, y, direction)) {
		return true;
	}

	if (m_op_state) {
		m_op_state->OnMouseWheelRotation(x, y, direction);
	}

	return false;
}

}