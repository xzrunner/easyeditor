#include "PixelDiffOP.h"
#include "StagePanel.h"

namespace eimage
{

PixelDiffOP::PixelDiffOP(StagePanel* stage)
	: d2d::ZoomViewOP(stage, true)
	, m_stage(stage)
{
}

bool PixelDiffOP::OnMouseLeftDown(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	if (!m_stage->getLeft() || !m_stage->getRight()) {
		return false;
	}

	const d2d::ISprite* s_left = m_stage->getLeft();
	const d2d::ISprite* s_right = m_stage->getRight();
	d2d::Image* left = static_cast<const d2d::ImageSymbol&>(static_cast<const d2d::ImageSprite*>(s_left)->GetSymbol()).getImage();
	d2d::Image* right = static_cast<const d2d::ImageSymbol&>(static_cast<const d2d::ImageSprite*>(s_right)->GetSymbol()).getImage();
	d2d::Vector p = m_stage->TransPosScrToProj(x, y);

	m_stage->getLeft()->GetSymbol().GetSize();

	int row, col;
	row = (p.y - left->GetOriginHeight() * 0.5f) / left->GetOriginHeight();

	if (p.x < 0) {

	}

	return false;
}

bool PixelDiffOP::OnDraw() const
{
// 	if (d2d::ZoomViewOP::OnDraw()) return true;
// 
// 	d2d::PrimitiveDraw::drawLine(d2d::Vector(0, -1024), d2d::Vector(0, 1024), d2d::LIGHT_GREY);

	return false;
}

bool PixelDiffOP::Clear()
{
	if (d2d::ZoomViewOP::Clear()) return true;
	return false;
}

bool PixelDiffOP::OnActive()
{
	if (d2d::ZoomViewOP::OnActive()) {
		return true;
	}

	d2d::ShaderMgr* shader_mgr = d2d::ShaderMgr::Instance();
	d2d::SpriteShader* shader = new d2d::SpriteShader;
	shader->Load();
	shader_mgr->SetSpriteShader(shader);

	m_stage->GetCanvas()->ResetViewport();

	return false;
}

}