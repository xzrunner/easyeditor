#include "StageCanvas.h"
#include "StagePanel.h"
#include "Symbol.h"

#include <ee/MultiSpritesImpl.h>
#include <ee/FetchAllVisitor.h>
#include <ee/render_utility.h>
#include <ee/Sprite.h>
#include <ee/SpriteRenderer.h>

namespace eshadow
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
	std::vector<ee::Sprite*> sprites;
	m_sprite_impl->TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprites));
	m_bg = ee::draw_all_to_one_spr(sprites, m_edited);
}

void StageCanvas::OnDrawSprites() const
{
	if (m_edited && m_bg) 
	{
		sm::mat4 mat(m_edited->GetTransInvMatrix());
		ee::SpriteRenderer::Draw(m_bg, NULL, mat);
	}

	m_stage->GetSymbol()->Draw(sm::mat4());
	m_stage->DrawEditOP();
}

}