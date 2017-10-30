#include "StageCanvas.h"
#include "StagePanel.h"
#include "Sprite.h"
#include "Symbol.h"

#include <ee/MultiSpritesImpl.h>
#include <ee/FetchAllVisitor.h>
#include <ee/SpriteRenderer.h>
#include <ee/render_utility.h>

#include <sprite2/RVG.h>
#include <sprite2/CameraType.h>

namespace eicon
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::CameraCanvas(stage, stage->GetStageImpl(), s2::CAM_ORTHO2D)
	, m_stage(stage)
	, m_edited(NULL)
	, m_sprite_impl(NULL)
	, m_bg(NULL)
{
}

StageCanvas::StageCanvas(StagePanel* stage, 
						 wxGLContext* glctx,
						 ee::SprPtr edited,
						 const ee::MultiSpritesImpl* bg_sprites)
	: ee::CameraCanvas(stage, stage->GetStageImpl(), s2::CAM_ORTHO2D, glctx)
	, m_stage(stage)
	, m_edited(edited)
	, m_sprite_impl(bg_sprites)
	, m_bg(NULL)
{
	if (m_sprite_impl) {
		std::vector<ee::SprPtr> sprs;
		m_sprite_impl->TraverseSprites(ee::FetchAllRefVisitor<ee::Sprite>(sprs));
		m_bg = ee::draw_all_to_one_spr(sprs, m_edited);
	}
}

void StageCanvas::OnDrawSprites() const
{
	if (m_edited && m_bg) 
	{
		s2::RenderParams params;
		params.mt = m_edited->GetLocalMat().Inverted();
		ee::SpriteRenderer::Instance()->Draw(m_bg.get(), params);
	}

	Sprite spr;
	spr.SetSymbol(m_stage->GetSymbol());
	spr.SetProcess(0.5f);

	m_stage->GetSymbol()->DrawTree(nullptr, s2::RenderParams(), &spr);

	s2::RVG::SetColor(s2::Color(255, 0, 0));
	s2::RVG::Cross(sm::vec2(0, 0), 100, 100);

	m_stage->DrawEditOP();
}

}