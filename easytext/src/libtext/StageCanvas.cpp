#include "StageCanvas.h"
#include "StagePanel.h"
#include "GTxt.h"
#include "Sprite.h"

namespace etext
{

StageCanvas::StageCanvas(StagePanel* stage)
	: d2d::OrthoCanvas(stage, stage->GetStageImpl())
	, m_stage(stage)
	, m_edited(NULL)
	, m_sprite_impl(NULL)
	, m_bg(NULL)
{
}

StageCanvas::StageCanvas(StagePanel* stage, d2d::ISprite* edited,
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
	if (d2d::Config::Instance()->IsUseDTex()) {
		d2d::DynamicTexAndFont::Instance()->ReloadTexture();
	}

	GTxt::Instance()->ReloadTexture();

	m_bg = d2d::draw_all_to_one_spr(m_sprite_impl, m_edited);
	ResetViewport();
}


void StageCanvas::OnDrawSprites() const
{
	if (m_edited && m_bg) 
	{
		d2d::Matrix mat(m_edited->GetTransInvMatrix());
		d2d::SpriteRenderer::Instance()->Draw(m_bg, mat);
	}

	d2d::SpriteRenderer::Instance()->Draw(m_edited);

	DrawSprBound();

	m_stage->DrawEditOP();

#ifdef _DEBUG 
	if (d2d::Config::Instance()->IsUseDTex()) {
		d2d::DynamicTexAndFont::Instance()->DebugDraw();
	}
#endif
}

void StageCanvas::DrawSprBound() const
{
	d2d::Matrix mt;
	m_edited->GetTransMatrix(mt);

	const Sprite* font = static_cast<const Sprite*>(m_edited);
	int w, h;
	font->GetSize(w, h);

	d2d::ShapeStyle style;
	style.fill = false;
	style.color.set(0.8f, 0.4f, 0.4f);

	d2d::PrimitiveDraw::rect(mt, w * 0.5f, h * 0.5f, style);
}

}