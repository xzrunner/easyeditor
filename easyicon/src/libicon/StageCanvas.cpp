#include "StageCanvas.h"
#include "StagePanel.h"
#include "Sprite.h"

namespace eicon
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::OrthoCanvas(stage, stage->GetStageImpl())
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
	: ee::OrthoCanvas(stage, stage->GetStageImpl(), glctx)
	, m_stage(stage)
	, m_edited(edited)
	, m_sprite_impl(bg_sprites)
	, m_bg(NULL)
{
	if (m_sprite_impl) {
		std::vector<Sprite*> sprites;
		m_sprite_impl->TraverseSprites(FetchAllVisitor<Sprite>(sprites));
		m_bg = ee::draw_all_to_one_spr(sprites, m_edited);
	}
}

void StageCanvas::OnDrawSprites() const
{
	if (m_edited && m_bg) 
	{
		ee::Matrix mat(m_edited->GetTransInvMatrix());
		ee::SpriteRenderer::Instance()->Draw(m_bg, NULL, mat);
	}

	Sprite sprite;
	sprite.SetSymbol(&m_stage->GetSymbol());
	sprite.SetProcess(0.5f);

	m_stage->GetSymbol().Draw(ee::Matrix(), ee::ColorTrans(), &sprite);

	ee::PrimitiveDraw::Cross(ee::Vector(0, 0), 100, 100, ee::Colorf(1, 0, 0));;

	m_stage->DrawEditOP();
}

}