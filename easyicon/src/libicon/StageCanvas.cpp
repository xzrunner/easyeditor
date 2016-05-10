#include "StageCanvas.h"
#include "StagePanel.h"
#include "Sprite.h"

#include <ee/MultiSpritesImpl.h>
#include <ee/FetchAllVisitor.h>
#include <ee/SpriteRenderer.h>
#include <ee/EE_RVG.h>
#include <ee/render_utility.h>

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

void StageCanvas::OnDrawSprites() const
{
	if (m_edited && m_bg) 
	{
		sm::mat4 mat(m_edited->GetTransInvMatrix());
		ee::SpriteRenderer::Draw(m_bg, NULL, mat);
	}

	Sprite sprite;
	sprite.SetSymbol(&m_stage->GetSymbol());
	sprite.SetProcess(0.5f);

	m_stage->GetSymbol().Draw(ee::RenderParams(), &sprite);

	ee::RVG::Color(s2::Color(255, 0, 0));
	ee::RVG::Cross(sm::vec2(0, 0), 100, 100);

	m_stage->DrawEditOP();
}

}