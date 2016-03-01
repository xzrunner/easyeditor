#include "PixelDiffOP.h"
#include "StagePanel.h"

#include <ee/Image.h>
#include <ee/ImageSymbol.h>
#include <ee/ImageSprite.h>
#include <ee/ShaderMgr.h>
#include <ee/SpriteShader.h>

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

	const ee::Sprite* s_left = m_stage->GetLeft();
	const ee::Sprite* s_right = m_stage->GetRight();
	ee::Image* left = static_cast<const ee::ImageSymbol&>(static_cast<const ee::ImageSprite*>(s_left)->GetSymbol()).GetImage();
	ee::Image* right = static_cast<const ee::ImageSymbol&>(static_cast<const ee::ImageSprite*>(s_right)->GetSymbol()).GetImage();
	ee::Vector p = m_stage->TransPosScrToProj(x, y);

	m_stage->GetLeft()->GetSymbol().GetSize();

	int row, col;
	row = (p.y - left->GetOriginHeight() * 0.5f) / left->GetOriginHeight();

	if (p.x < 0) {

	}

	return false;
}

bool PixelDiffOP::OnDraw() const
{
// 	if (ee::ZoomViewOP::OnDraw()) return true;
// 
// 	ee::RVG::Line(ee::Vector(0, -1024), ee::Vector(0, 1024), ee::LIGHT_GREY);

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

	ee::ShaderMgr* shader_mgr = ee::ShaderMgr::Instance();
	ee::SpriteShader* shader = new ee::SpriteShader;
	shader->Load();
	shader_mgr->SetSpriteShader(shader);

	return false;
}

}