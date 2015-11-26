#include "StageCanvas.h"
#include "StagePanel.h"
#include "Sprite.h"

namespace eicon
{

StageCanvas::StageCanvas(StagePanel* stage)
	: d2d::OrthoCanvas(stage, stage->GetStageImpl())
	, m_stage(stage)
	, m_edited(NULL)
	, m_sprite_impl(NULL)
	, m_bg(NULL)
{
}

StageCanvas::StageCanvas(StagePanel* stage, 
						 d2d::ISprite* edited,
						 const d2d::MultiSpritesImpl* bg_sprites)
	: d2d::OrthoCanvas(stage, stage->GetStageImpl())
	, m_stage(stage)
	, m_edited(edited)
	, m_sprite_impl(bg_sprites)
	, m_bg(NULL)
{
}

void StageCanvas::InitGL()
{
	d2d::OrthoCanvas::InitGL();

	d2d::ImageMgr::Instance()->Traverse(d2d::ReloadTextureVisitor<d2d::Image>());

	if (m_sprite_impl) {
		m_bg = d2d::draw_all_to_one_spr(m_sprite_impl, m_edited);
	}
	d2d::ResetViewportSJ::Instance()->Reset();
}

void StageCanvas::OnDrawSprites() const
{
	if (m_edited && m_bg) 
	{
		d2d::Matrix mat(m_edited->GetTransInvMatrix());
		d2d::SpriteRenderer::Instance()->Draw(m_bg, mat);
	}

	Sprite sprite;
	sprite.SetSymbol(&m_stage->GetSymbol());
	sprite.SetProcess(0.5f);

	m_stage->GetSymbol().Draw(d2d::Matrix(), d2d::Colorf(1, 1, 1, 1), d2d::Colorf(0, 0, 0, 0),
		d2d::Colorf(1, 0, 0, 0), d2d::Colorf(0, 1, 0, 0), d2d::Colorf(0, 0, 1, 0), &sprite);

	d2d::PrimitiveDraw::cross(d2d::Vector(0, 0), 100, 100, d2d::Colorf(1, 0, 0));;

	m_stage->DrawEditOP();
}

}