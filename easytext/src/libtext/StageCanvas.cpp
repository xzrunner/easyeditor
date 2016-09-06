#include "StageCanvas.h"
#include "StagePanel.h"
#include "GTxt.h"
#include "Sprite.h"

#include <ee/FetchAllVisitor.h>
#include <ee/render_utility.h>
#include <ee/SpriteRenderer.h>
#include <ee/Config.h>
#include <ee/Math2D.h>

#include <sprite2/S2_RVG.h>
#include <glue/GLUE_DTex.h>

namespace etext
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::CameraCanvas(stage, stage->GetStageImpl())
	, m_stage(stage)
	, m_edited(NULL)
	, m_sprite_impl(NULL)
	, m_bg(NULL)
{
}

StageCanvas::StageCanvas(StagePanel* stage, wxGLContext* glctx,
						 ee::Sprite* edited, const ee::MultiSpritesImpl* bg_sprites)
	: ee::CameraCanvas(stage, stage->GetStageImpl(), glctx)
	, m_stage(stage)
	, m_edited(edited)
	, m_sprite_impl(bg_sprites)
	, m_bg(NULL)
{
	std::vector<ee::Sprite*> sprs;
	m_sprite_impl->TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprs));
	m_bg = ee::draw_all_to_one_spr(sprs, m_edited);
}

StageCanvas::~StageCanvas()
{
	if (m_bg) {
		m_bg->RemoveReference();
	}
}

void StageCanvas::OnDrawSprites() const
{
	if (m_edited && m_bg) 
	{
		s2::RenderParams params;
		params.mt = m_edited->GetTransInvMatrix();
		ee::SpriteRenderer::Instance()->Draw(m_bg, params);
	}

	ee::SpriteRenderer::Instance()->Draw(m_edited);

	DrawSprBound();

	m_stage->DrawEditOP();

#ifdef _DEBUG 
	if (ee::Config::Instance()->IsUseDTex()) {
		glue::DTex::Instance()->DebugDraw();
	}
#endif
}

void StageCanvas::DrawSprBound() const
{
	s2::RVG::SetColor(s2::Color(204, 102, 102));

	const Sprite* font = static_cast<const Sprite*>(m_edited);
	const s2::Textbox& tb = font->GetTextbox();
	float hw = tb.width * 0.5f,
		  hh = tb.height * 0.5f;

	sm::mat4 mt = m_edited->GetTransMatrix();
	sm::vec2 min(-hw, -hh), max(hw, hh);
	min = mt * min;
	max = mt * max;

	s2::RVG::Rect(min, max, false);
}

}