//#include "NodeArrangeOP.h"
//#include "StagePanel.h"
//#include "StageCanvas.h"
//
//#include "NodeTranslateState.h"
//#include "NodeRotateState.h"
//#include "CamTranslateState.h"
//#include "CamRotateState.h"
//#include "CamZoomState.h"
//
//#include <ee/FetchAllVisitor.h>
//
//#include <sprite2/ModelSprite.h>
//
//namespace enode3d
//{
//
//NodeArrangeOP::NodeArrangeOP(StagePanel& stage)
//	: NodeSelectOP(stage)
//{
//	m_canvas = static_cast<enode3d::StageCanvas*>(stage.GetCanvas());
//}
//
//bool NodeArrangeOP::OnKeyDown(int keyCode)
//{
//	if (NodeSelectOP::OnKeyDown(keyCode)) {
//		return true;
//	}
//
//	switch (keyCode)
//	{
//	case WXK_ESCAPE:
//		m_canvas->GetCamera().Reset();
//		break;
//	case WXK_SPACE:
//		{
//			std::vector<n3::NodePtr> nodes;
//			m_stage.GetNodeSelection().Traverse(
//				ee::FetchAllRefVisitor<n3::INode>(nodes));
//			for (auto& node : nodes) 
//			{
//				node->SetPos(sm::vec3(0, 0, 0));
//				node->SetAngle(sm::Quaternion());
//				node->SetScale(sm::vec3(1, 1, 1));
//			}
//		}
//		break;
//	}
//
//	return false;
//}
//
//bool NodeArrangeOP::OnMouseLeftDown(int x, int y)
//{
//	if (NodeSelectOP::OnMouseLeftDown(x, y)) {
//		return true;
//	}
//
//	auto& selection = m_stage.GetNodeSelection();
//	if (selection.IsEmpty()) {
//		m_op_state = std::make_unique<CamRotateState>(
//			m_canvas->GetCamera(), sm::vec2(x, y));
//	} else {
//		m_op_state = std::make_unique<NodeTranslateState>(
//			*m_canvas, selection);
//	}
//
//	if (m_op_state) {
//		m_op_state->OnMousePress(sm::vec2(x, y));
//	}
//
//	return false;
//}
//
//bool NodeArrangeOP::OnMouseLeftUp(int x, int y)
//{
//	if (NodeSelectOP::OnMouseLeftUp(x, y)) {
//		return true;
//	}
//
//	if (m_op_state) {
//		m_op_state->OnMouseRelease(sm::vec2(x, y));
//	}
//
//	m_op_state = std::make_unique<CamZoomState>(
//		m_canvas->GetCamera(), m_canvas->GetViewport());
//
//	return false;
//}
//
//bool NodeArrangeOP::OnMouseRightDown(int x, int y)
//{
//	if (NodeSelectOP::OnMouseRightDown(x, y)) {
//		return true;
//	}
//
//	auto& selection = m_stage.GetNodeSelection();
//	if (selection.IsEmpty()) {
//		m_op_state = std::make_unique<CamTranslateState>(
//			m_canvas->GetCamera(), sm::vec2(x, y));
//	} else if (selection.Size() == 1) {
//		m_op_state = std::make_unique<NodeRotateState>(
//			*m_canvas, selection);
//	}
//
//	if (m_op_state) {
//		m_op_state->OnMousePress(sm::vec2(x, y));
//	}
//
//	return false;
//}
//
//bool NodeArrangeOP::OnMouseRightUp(int x, int y)
//{
//	if (NodeSelectOP::OnMouseRightUp(x, y)) {
//		return true;
//	}
//
//	if (m_op_state) {
//		m_op_state->OnMouseRelease(sm::vec2(x, y));
//	}
//
//	m_op_state = std::make_unique<CamZoomState>(
//		m_canvas->GetCamera(), m_canvas->GetViewport());
//
//	return false;
//}
//
//bool NodeArrangeOP::OnMouseMove(int x, int y)
//{
//	if (NodeSelectOP::OnMouseMove(x, y)) {
//		return true;
//	}
//
//	//m_stage->SetFocus();
//
//	if (m_op_state) {
//		m_op_state->OnMouseMove(sm::vec2(x, y));
//	}
//
//	return false;
//}
//
//bool NodeArrangeOP::OnMouseDrag(int x, int y)
//{
//	if (NodeSelectOP::OnMouseDrag(x, y)) {
//		return true;
//	}
//
//	if (m_op_state) {
//		m_op_state->OnMouseDrag(sm::vec2(x, y));
//	}
//
//	return false;
//}
//
//bool NodeArrangeOP::OnMouseWheelRotation(int x, int y, int direction)
//{
//	if (NodeSelectOP::OnMouseWheelRotation(x, y, direction)) {
//		return true;
//	}
//
//	if (m_op_state) {
//		m_op_state->OnMouseWheelRotation(x, y, direction);
//	}
//
//	return false;
//}
//
//}