#include "ArrangeSpriteOP.h"
#include "StagePanel.h"
#include "IEditState.h"
#include "Sprite.h"

#include "TranslateSpriteState.h"
#include "RotateSpriteState.h"
#include "TranslateCameraState.h"
#include "RotateCameraState.h"

#include <ee/FetchAllVisitor.h>

namespace esketch
{

ArrangeSpriteOP::ArrangeSpriteOP(StagePanel* stage)
	: SelectSpriteOP(stage)
	, m_state(NULL)
{
	const ee::SpriteSelection& selection = GetSelection();
	m_translate_sprite = new TranslateSpriteState(m_stage, selection);
	m_rotate_sprite = new RotateSpriteState(m_stage, selection);

	e3d::StageCanvas* canvas 
		= static_cast<e3d::StageCanvas*>(m_stage->GetCanvas());
	m_translate_camera = new TranslateCameraState(canvas);
	m_rotate_camera = new RotateCameraState(canvas);
}

ArrangeSpriteOP::~ArrangeSpriteOP()
{
	delete m_translate_sprite;
	delete m_rotate_sprite;
	delete m_translate_camera;
	delete m_rotate_camera;
}

bool ArrangeSpriteOP::OnKeyDown(int keyCode)
{
	if (SelectSpriteOP::OnKeyDown(keyCode)) {
		return true;
	}

	switch (keyCode)
	{
	case WXK_SPACE:
		{
			std::vector<Sprite*> sprs;
			GetSelection().Traverse(ee::FetchAllVisitor<Sprite>(sprs));
			for (int i = 0, n = sprs.size(); i < n; ++i) {
				Sprite* spr = sprs[i];
				spr->SetPos3(sm::vec3(0, 0, 0));
				spr->SetOri3(sm::Quaternion());
			}

			e3d::StageCanvas* canvas 
				= static_cast<e3d::StageCanvas*>(m_stage->GetCanvas());
			canvas->GetCamera3().Reset();
		}
		break;
	}

	return false;
}

bool ArrangeSpriteOP::OnMouseLeftDown(int x, int y)
{
	if (SelectSpriteOP::OnMouseLeftDown(x, y)) {
		return true;
	}

	if (GetSelection().IsEmpty()) {
		m_state = m_translate_camera;
	} else {
		m_state = m_translate_sprite;
	}

	if (m_state) {
		m_state->OnMousePress(sm::ivec2(x, y));
	}

	return false;
}

bool ArrangeSpriteOP::OnMouseLeftUp(int x, int y)
{
	if (SelectSpriteOP::OnMouseLeftUp(x, y)) {
		return true;
	}

	if (m_state) {
		m_state->OnMouseRelease(sm::ivec2(x, y));
	}

	return false;
}

bool ArrangeSpriteOP::OnMouseRightDown(int x, int y)
{
	if (SelectSpriteOP::OnMouseRightDown(x, y)) {
		return true;
	}

	const ee::SpriteSelection& selection = GetSelection();
	if (selection.IsEmpty()) {
		m_state = m_rotate_camera;
	} else if (selection.Size() == 1) {
		m_state = m_rotate_sprite;
	}

	if (m_state) {
		m_state->OnMousePress(sm::ivec2(x, y));
	}

	return false;
}

bool ArrangeSpriteOP::OnMouseRightUp(int x, int y)
{
	if (SelectSpriteOP::OnMouseRightUp(x, y)) {
		return true;
	}

	if (m_state) {
		m_state->OnMouseRelease(sm::ivec2(x, y));
	}

	return false;
}

bool ArrangeSpriteOP::OnMouseMove(int x, int y)
{
	if (SelectSpriteOP::OnMouseMove(x, y)) {
		return true;
	}

	m_stage->SetFocus();

	return false;
}

bool ArrangeSpriteOP::OnMouseDrag(int x, int y)
{
	if (SelectSpriteOP::OnMouseDrag(x, y)) {
		return true;
	}

	if (m_state) {
		m_state->OnMouseMove(sm::ivec2(x, y));
	}

	return false;
}

}