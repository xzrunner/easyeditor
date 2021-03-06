#include "StageCanvas.h"
#include "StagePanel.h"
#include "Sprite.h"

#include <ee/FetchAllVisitor.h>
#include <ee/render_utility.h>
#include <ee/SpriteRenderer.h>
#include <ee/Config.h>
#include <ee/Math2D.h>

#include <sprite2/RVG.h>
#include <sprite2/CameraType.h>

namespace etext
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::CameraCanvas(stage, stage->GetStageImpl(), s2::CAM_ORTHO2D)
	, m_stage(stage)
	, m_edited(NULL)
	, m_sprite_impl(NULL)
	, m_bg(NULL)
{
}

StageCanvas::StageCanvas(StagePanel* stage, wxGLContext* glctx,
						 ee::SprPtr edited, const ee::MultiSpritesImpl* bg_sprites)
	: ee::CameraCanvas(stage, stage->GetStageImpl(), s2::CAM_ORTHO2D, glctx)
	, m_stage(stage)
	, m_edited(edited)
	, m_sprite_impl(bg_sprites)
	, m_bg(NULL)
{
	std::vector<ee::SprPtr> sprs;
	m_sprite_impl->TraverseSprites(ee::FetchAllRefVisitor<ee::Sprite>(sprs));
	m_bg = ee::draw_all_to_one_spr(sprs, m_edited);
}

void StageCanvas::OnDrawSprites() const
{
	if (m_edited && m_bg) 
	{
		s2::RenderParams params;
		params.mt = m_edited->GetLocalMat().Inverted();
		ee::SpriteRenderer::Instance()->Draw(m_bg.get(), params);
	}

	ee::SpriteRenderer::Instance()->Draw(m_edited.get());

	DrawSprBound();

	m_stage->DrawEditOP();
}

void StageCanvas::DrawSprBound() const
{
	s2::RVG::SetColor(pt2::Color(204, 102, 102));

	auto font = std::dynamic_pointer_cast<Sprite>(m_edited);
	const s2::Textbox& tb = font->GetTextbox();
	float hw = tb.width * 0.5f,
		  hh = tb.height * 0.5f;

	S2_MAT mt = m_edited->GetLocalMat();
	sm::vec2 min(-hw, -hh), max(hw, hh);
	min = mt * min;
	max = mt * max;

	s2::RVG::Rect(nullptr, min, max, false);
}

}