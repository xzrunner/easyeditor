#include "StageCanvas.h"
#include "StagePanel.h"
#include "Sprite.h"
#include "Symbol.h"

#include <ee/MultiSpritesImpl.h>
#include <ee/FetchAllVisitor.h>
#include <ee/SpriteRenderer.h>
#include <ee/render_utility.h>

#include <sprite2/S2_RVG.h>

namespace eicon
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::CameraCanvas(stage, stage->GetStageImpl())
	, m_stage(stage)
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
	, m_edited(edited)
	, m_sprite_impl(bg_sprites)
	, m_bg(NULL)
{
	if (m_sprite_impl) {
		std::vector<ee::Sprite*> sprites;
		m_sprite_impl->TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprites));
		m_bg = ee::draw_all_to_one_spr(sprites, m_edited);
	}
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
		ee::SpriteRenderer::Draw(m_bg, params);
	}

	Sprite sprite;
	sprite.SetSymbol(&m_stage->GetSymbol());
	sprite.SetProcess(0.5f);

	m_stage->GetSymbol().Draw(s2::RenderParams(), &sprite);

	s2::RVG::SetColor(s2::Color(255, 0, 0));
	s2::RVG::Cross(sm::vec2(0, 0), 100, 100);

	m_stage->DrawEditOP();
}

}