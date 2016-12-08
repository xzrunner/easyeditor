#include "StageCanvas.h"
#include "StagePanel.h"

#include <ee/MultiSpritesImpl.h>
#include <ee/FetchAllVisitor.h>
#include <ee/render_utility.h>
#include <ee/cfg_const.h>
#include <ee/color_config.h>
#include <ee/Sprite.h>
#include <ee/DrawShapesVisitor.h>
#include <ee/SpriteRenderer.h>
#include <ee/Symbol.h>

#include <sprite2/S2_RVG.h>

namespace eshape
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::CameraCanvas(stage, stage->GetStageImpl())
	, m_stage(stage)
	, m_shape_impl(stage)
	, m_edited(NULL)
	, m_sprite_impl(NULL)
	, m_bg(NULL)
{
}

StageCanvas::StageCanvas(StagePanel* stage, 
						 wxGLContext* glctx,
						 ee::Sprite* edited,
						 const ee::MultiSpritesImpl* bg_sprites)
	: ee::CameraCanvas(stage, stage->GetStageImpl(), glctx)
	, m_stage(stage)
	, m_shape_impl(stage)
	, m_edited(edited)
	, m_sprite_impl(bg_sprites)
	, m_bg(NULL)
{
	if (m_sprite_impl) {
		std::vector<ee::Sprite*> sprs;
		m_sprite_impl->TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprs));
		m_bg = ee::draw_all_to_one_spr(sprs, m_edited);
	}
}

StageCanvas::~StageCanvas()
{
	if (m_bg) {
		m_bg->RemoveReference();
	}
}

void StageCanvas::DrawGuideLines() const
{
	s2::RVG::SetColor(ee::LIGHT_GREY);
	s2::RVG::Rect(sm::vec2(0, 0), ee::HALF_SCREEN_WIDTH, ee::HALF_SCREEN_HEIGHT, false);
}

void StageCanvas::OnDrawSprites() const
{
	if (m_edited && m_bg) 
	{
		s2::RenderParams params;
		params.mt = m_edited->GetLocalInvMat();
		ee::SpriteRenderer::Instance()->Draw(m_bg, params);
	}

	m_shape_impl->TraverseShapes(ee::DrawShapesVisitor(sm::rect()), ee::DT_VISIBLE);

	StageCanvas::DrawGuideLines();

	if (!m_edited) {
		m_stage->GetSymbol().Draw(s2::RenderParams());
	}

	m_stage->DrawEditOP();
}

}