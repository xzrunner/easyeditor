#include "ArrangeOP.h"
#include "StagePanel.h"
#include "IArrangeState.h"
#include "RotateCameraState.h"
#include "RotateSpriteState.h"
#include "Sprite.h"

namespace libsketch
{

ArrangeOP::ArrangeOP(StagePanel* stage)
	: SelectOP(stage)
	, m_state(NULL)
{
// 	e3d::StageCanvas* canvas = static_cast<e3d::StageCanvas*>(stage->getCanvas());
// 	m_state = new RotateCameraState(canvas);
}

ArrangeOP::~ArrangeOP()
{
	delete m_state;
}

bool ArrangeOP::onMouseLeftDown(int x, int y)
{
	if (SelectOP::onMouseLeftDown(x, y)) {
		return true;
	}

	// after select, set state
	const d2d::SpriteSelection& selection = GetSelection();
	if (!m_state && !selection.empty()) {
		const e3d::StageCanvas* canvas 
			= static_cast<const e3d::StageCanvas*>(m_stage->getCanvas());
		std::vector<d2d::ISprite*> sprites;
		selection.traverse(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
//  		m_state = new RotateSpriteState(const_cast<e3d::StageCanvas*>(canvas), 
//  			static_cast<Sprite*>(sprites[0]));
		m_state = new RotateCameraState(const_cast<e3d::StageCanvas*>(canvas));
	}

	if (m_state) {
		m_state->OnMousePress(ivec2(x, y));
	}

	return false;
}

bool ArrangeOP::onMouseLeftUp(int x, int y)
{
	if (SelectOP::onMouseLeftUp(x, y)) {
		return true;
	}

	if (m_state) {
		m_state->OnMouseRelease(ivec2(x, y));
	}

	return false;
}

bool ArrangeOP::onMouseRightDown(int x, int y)
{
	if (SelectOP::onMouseRightDown(x, y)) {
		return true;
	}

	if (m_state) {
		m_state->OnMousePress(ivec2(x, y));
	}

	return false;
}

bool ArrangeOP::onMouseRightUp(int x, int y)
{
	if (SelectOP::onMouseRightUp(x, y)) {
		return true;
	}

	if (m_state) {
		m_state->OnMouseRelease(ivec2(x, y));
	}

	return false;
}

bool ArrangeOP::onMouseDrag(int x, int y)
{
	if (SelectOP::onMouseDrag(x, y)) {
		return true;
	}

	if (m_state) {
		m_state->OnMouseMove(ivec2(x, y));
	}

	return false;
}

}