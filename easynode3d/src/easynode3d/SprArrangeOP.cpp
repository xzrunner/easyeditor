#include "SprArrangeOP.h"
#include "StagePanel.h"
#include "StageCanvas.h"

#include "SprTranslateState.h"
#include "SprRotateState.h"
#include "CamTranslateState.h"
#include "CamRotateState.h"
#include "CamZoomState.h"

#include <ee/FetchAllVisitor.h>

#include <sprite2/ModelSprite.h>

namespace enode3d
{

SprArrangeOP::SprArrangeOP(StagePanel& stage)
	: SprSelectOP(stage)
{
	m_canvas = static_cast<enode3d::StageCanvas*>(stage.GetCanvas());
}

bool SprArrangeOP::OnKeyDown(int keyCode)
{
	if (SprSelectOP::OnKeyDown(keyCode)) {
		return true;
	}

	switch (keyCode)
	{
	case WXK_ESCAPE:
		m_canvas->GetCamera().Reset(sm::vec3(0, 0, -2), sm::vec3(0, 0, 0), sm::vec3(0, 1, 0));
		break;
	case WXK_SPACE:
		{
			std::vector<ee::SprPtr> sprs;
			GetSelection().Traverse(ee::FetchAllRefVisitor<ee::Sprite>(sprs));
			for (auto& spr : sprs) 
			{
				auto model_spr = std::dynamic_pointer_cast<s2::ModelSprite>(spr);
				model_spr->SetPos3(sm::vec3(0, 0, 0));
				model_spr->SetOri3(sm::Quaternion());
			}
		}
		break;
	}

	return false;
}

bool SprArrangeOP::OnMouseLeftDown(int x, int y)
{
	if (SprSelectOP::OnMouseLeftDown(x, y)) {
		return true;
	}

	if (GetSelection().IsEmpty()) {
		m_op_state = std::make_unique<CamRotateState>(
			*m_canvas, m_canvas->GetCamera(), sm::vec2(x, y));
	} else {
		m_op_state = std::make_unique<SprTranslateState>(
			*m_canvas, GetSelection());
	}

	if (m_op_state) {
		m_op_state->OnMousePress(sm::vec2(x, y));
	}

	return false;
}

bool SprArrangeOP::OnMouseLeftUp(int x, int y)
{
	if (SprSelectOP::OnMouseLeftUp(x, y)) {
		return true;
	}

	if (m_op_state) {
		m_op_state->OnMouseRelease(sm::vec2(x, y));
	}

	m_op_state = std::make_unique<CamZoomState>(
		*m_canvas, m_canvas->GetCamera());

	return false;
}

bool SprArrangeOP::OnMouseRightDown(int x, int y)
{
	if (SprSelectOP::OnMouseRightDown(x, y)) {
		return true;
	}

	const ee::SpriteSelection& selection = GetSelection();
	if (selection.IsEmpty()) {
		m_op_state = std::make_unique<CamTranslateState>(
			*m_canvas, m_canvas->GetCamera(), sm::vec2(x, y));
	} else if (selection.Size() == 1) {
		m_op_state = std::make_unique<SprRotateState>(
			*m_canvas, GetSelection());
	}

	if (m_op_state) {
		m_op_state->OnMousePress(sm::vec2(x, y));
	}

	return false;
}

bool SprArrangeOP::OnMouseRightUp(int x, int y)
{
	if (SprSelectOP::OnMouseRightUp(x, y)) {
		return true;
	}

	if (m_op_state) {
		m_op_state->OnMouseRelease(sm::vec2(x, y));
	}

	m_op_state = std::make_unique<CamZoomState>(
		*m_canvas, m_canvas->GetCamera());

	return false;
}

bool SprArrangeOP::OnMouseMove(int x, int y)
{
	if (SprSelectOP::OnMouseMove(x, y)) {
		return true;
	}

	//m_stage->SetFocus();

	if (m_op_state) {
		m_op_state->OnMouseMove(sm::vec2(x, y));
	}

	return false;
}

bool SprArrangeOP::OnMouseDrag(int x, int y)
{
	if (SprSelectOP::OnMouseDrag(x, y)) {
		return true;
	}

	if (m_op_state) {
		m_op_state->OnMouseDrag(sm::vec2(x, y));
	}

	return false;
}

bool SprArrangeOP::OnMouseWheelRotation(int x, int y, int direction)
{
	if (SprSelectOP::OnMouseWheelRotation(x, y, direction)) {
		return true;
	}

	if (m_op_state) {
		m_op_state->OnMouseWheelRotation(x, y, direction);
	}

	return false;
}

}