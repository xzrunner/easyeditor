#include "ArrangeSpriteOP.h"
#include "StagePanel.h"
#include "IEditState.h"
#include "Sprite.h"

#include "TranslateSpriteState.h"
#include "RotateSpriteState.h"
#include "TranslateCameraState.h"
#include "RotateCameraState.h"

namespace libsketch
{

ArrangeSpriteOP::ArrangeSpriteOP(StagePanel* stage)
	: SelectSpriteOP(stage)
	, m_state(NULL)
{
	const d2d::SpriteSelection& selection = GetSelection();
	m_translate_sprite = new TranslateSpriteState(m_stage, selection);
	m_rotate_sprite = new RotateSpriteState(m_stage, selection);

	e3d::StageCanvas* canvas 
		= static_cast<e3d::StageCanvas*>(m_stage->getCanvas());
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

bool ArrangeSpriteOP::onKeyDown(int keyCode)
{
	if (SelectSpriteOP::onKeyDown(keyCode)) {
		return true;
	}

	switch (keyCode)
	{
	case WXK_SPACE:
		{
			std::vector<Sprite*> sprites;
			GetSelection().traverse(d2d::FetchAllVisitor<Sprite>(sprites));
			for (int i = 0, n = sprites.size(); i < n; ++i) {
				Sprite* sprite = sprites[i];
				sprite->SetPos3(vec3(0, 0, 0));
				sprite->SetOri3(Quaternion());
			}
		}
		break;
	}

	return false;
}

bool ArrangeSpriteOP::onMouseLeftDown(int x, int y)
{
	if (SelectSpriteOP::onMouseLeftDown(x, y)) {
		return true;
	}

	if (GetSelection().empty()) {
		m_state = m_translate_camera;
	} else {
		m_state = m_translate_sprite;
	}

	if (m_state) {
		m_state->OnMousePress(ivec2(x, y));
	}

	return false;
}

bool ArrangeSpriteOP::onMouseLeftUp(int x, int y)
{
	if (SelectSpriteOP::onMouseLeftUp(x, y)) {
		return true;
	}

	if (m_state) {
		m_state->OnMouseRelease(ivec2(x, y));
	}

	return false;
}

bool ArrangeSpriteOP::onMouseRightDown(int x, int y)
{
	if (SelectSpriteOP::onMouseRightDown(x, y)) {
		return true;
	}

	const d2d::SpriteSelection& selection = GetSelection();
	if (selection.empty()) {
		m_state = m_rotate_camera;
	} else if (selection.size() == 1) {
		m_state = m_rotate_sprite;
	}

	if (m_state) {
		m_state->OnMousePress(ivec2(x, y));
	}

	return false;
}

bool ArrangeSpriteOP::onMouseRightUp(int x, int y)
{
	if (SelectSpriteOP::onMouseRightUp(x, y)) {
		return true;
	}

	if (m_state) {
		m_state->OnMouseRelease(ivec2(x, y));
	}

	return false;
}

bool ArrangeSpriteOP::onMouseDrag(int x, int y)
{
	if (SelectSpriteOP::onMouseDrag(x, y)) {
		return true;
	}

	if (m_state) {
		m_state->OnMouseMove(ivec2(x, y));
	}

	return false;
}

}