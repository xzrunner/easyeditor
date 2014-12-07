#include "PixelDiffOP.h"
#include "StagePanel.h"

namespace eimage
{

PixelDiffOP::PixelDiffOP(StagePanel* stage)
	: d2d::ZoomViewOP(stage, true)
	, m_stage(stage)
{
}

bool PixelDiffOP::onMouseLeftDown(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseLeftDown(x, y)) return true;

	if (!m_stage->getLeft() || !m_stage->getRight()) {
		return false;
	}

	const d2d::ISprite* s_left = m_stage->getLeft();
	const d2d::ISprite* s_right = m_stage->getRight();
	d2d::Image* left = static_cast<const d2d::ImageSymbol&>(static_cast<const d2d::ImageSprite*>(s_left)->getSymbol()).getImage();
	d2d::Image* right = static_cast<const d2d::ImageSymbol&>(static_cast<const d2d::ImageSprite*>(s_right)->getSymbol()).getImage();
	d2d::Vector p = m_editPanel->transPosScreenToProject(x, y);

	m_stage->getLeft()->getSymbol().getSize();

	int row, col;
	row = (p.y - left->originHeight() * 0.5f) / left->originHeight();

	if (p.x < 0) {

	}

	return false;
}

bool PixelDiffOP::onDraw() const
{
// 	if (d2d::ZoomViewOP::onDraw()) return true;
// 
// 	d2d::PrimitiveDraw::drawLine(d2d::Vector(0, -1024), d2d::Vector(0, 1024), d2d::LIGHT_GREY);

	return false;
}

bool PixelDiffOP::clear()
{
	if (d2d::ZoomViewOP::clear()) return true;
	return false;
}

bool PixelDiffOP::onActive()
{
	if (d2d::ZoomViewOP::onActive()) {
		return true;
	}

	d2d::ShaderMgr* shader_mgr = d2d::ShaderMgr::Instance();
	d2d::SpriteShader* shader = new d2d::SpriteShader;
	shader->Load();
	shader_mgr->SetSpriteShader(shader);

	m_stage->getCanvas()->resetViewport();

	return false;
}

}