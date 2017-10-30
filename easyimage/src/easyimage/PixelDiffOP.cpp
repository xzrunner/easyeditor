#include "PixelDiffOP.h"
#include "StagePanel.h"

#include <ee/Image.h>
#include <ee/ImageSymbol.h>
#include <ee/ImageSprite.h>

namespace eimage
{

PixelDiffOP::PixelDiffOP(StagePanel* stage)
	: ee::ZoomViewOP(stage, stage->GetStageImpl(), true)
	, m_stage(stage)
{
}

bool PixelDiffOP::OnMouseLeftDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	if (!m_stage->GetLeft() || !m_stage->GetRight()) {
		return false;
	}

	const ee::SprPtr s_left = m_stage->GetLeft();
	const ee::SprPtr s_right = m_stage->GetRight();
	ee::Image* left = std::dynamic_pointer_cast<const ee::ImageSymbol>>(static_cast<const std::shared_ptr<const ee::ImageSprite>>(s_left)->GetSymbol())->GetImage();
	ee::Image* right = std::dynamic_pointer_cast<const ee::ImageSymbol>>(static_cast<const std::shared_ptr<const ee::ImageSprite>>(s_right)->GetSymbol())->GetImage();
	sm::vec2 p = m_stage->TransPosScrToProj(x, y);

	m_stage->GetLeft()->GetSymbol()->GetBounding();

	int row, col;
	row = (p.y - left->GetOriginSize().y * 0.5f) / left->GetOriginSize().y;

	if (p.x < 0) {

	}

	return false;
}

bool PixelDiffOP::OnDraw() const
{
// 	if (ee::ZoomViewOP::OnDraw()) return true;
// 
// 	s2::RVG::Line(sm::vec2(0, -1024), sm::vec2(0, 1024), ee::LIGHT_GREY);

	return false;
}

bool PixelDiffOP::Clear()
{
	if (ee::ZoomViewOP::Clear()) return true;
	return false;
}

bool PixelDiffOP::OnActive()
{
	if (ee::ZoomViewOP::OnActive()) {
		return true;
	}

// 	ee::ShaderMgr* shader_mgr = ee::ShaderMgr::Instance();
// 	ee::SpriteShader* shader = new ee::SpriteShader;
// 	shader->Load();
//	shader_mgr->SetSpriteShader(shader);

	return false;
}

}