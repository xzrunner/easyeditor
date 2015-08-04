#include "StageCanvas.h"
#include "StagePanel.h"
#include "Symbol.h"

namespace etexture
{

StageCanvas::StageCanvas(StagePanel* panel)
	: d2d::OrthoCanvas(panel)
	, m_panel(panel)
	, m_edited(NULL)
	, m_sprite_impl(NULL)
	, m_bg(NULL)
{
}

StageCanvas::StageCanvas(StagePanel* panel, d2d::ISprite* edited, 
						 const d2d::MultiSpritesImpl* bg_sprites)
	: d2d::OrthoCanvas(panel)
	, m_panel(panel)
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

	d2d::Rect sr = m_screen.GetRegion();
	m_panel->TraverseSprites(d2d::DrawSpritesVisitor(sr, m_camera->GetScale()), 
		d2d::DT_VISIBLE);
	m_panel->TraverseShapes(d2d::DrawShapesVisitor(sr), d2d::DT_VISIBLE);

	d2d::PrimitiveDraw::cross(d2d::Vector(0, 0), 100, 100, d2d::Colorf(1, 0, 0));

	m_stage->DrawEditOP();
}

}