#include "editop/NodeArrangeOP.h"
#include "view/StagePanel.h"
#include "view/StageCanvas.h"

#include "editop/NodeTranslateState.h"
#include "editop/NodeRotateState.h"

#include <ee/FetchAllVisitor.h>

#include <easynode3d/CamTranslateState.h>
#include <easynode3d/CamRotateState.h>
#include <easynode3d/CamZoomState.h>

#include <sprite2/ModelSprite.h>

namespace eone
{

NodeArrangeOP::NodeArrangeOP(StagePanel& stage)
	: NodeSelectOP(stage)
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
				[](const SceneNodePtr& node)->bool
				{
					auto& ctrans = node->GetComponent<n3::CompTransform>();
					ctrans.SetPosition(sm::vec3(0, 0, 0));
					ctrans.SetAngle(sm::Quaternion());
					ctrans.SetScale(sm::vec3(1, 1, 1));
					return true;
				}
			);
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
		m_op_state = std::make_unique<enode3d::CamRotateState>(
			m_canvas->GetCamera(), sm::vec2(x, y));
	} else {
		m_op_state = std::make_unique<NodeTranslateState>(
			*m_canvas, m_stage.GetSubjectMgr(), selection);
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

	m_op_state = std::make_unique<enode3d::CamZoomState>(
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
		m_op_state = std::make_unique<enode3d::CamTranslateState>(
			m_canvas->GetCamera(), sm::vec2(x, y));
	} else if (selection.Size() == 1) {
		m_op_state = std::make_unique<NodeRotateState>(
			*m_canvas, m_stage.GetSubjectMgr(), selection);
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

	m_op_state = std::make_unique<enode3d::CamZoomState>(
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