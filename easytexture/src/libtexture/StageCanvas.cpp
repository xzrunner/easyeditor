#include "StageCanvas.h"
#include "StagePanel.h"
#include "Symbol.h"

namespace etexture
{

StageCanvas::StageCanvas(StagePanel* panel)
	: d2d::OrthoCanvas(panel, panel->GetStageImpl())
	, m_panel(panel)
	, m_edited(NULL)
	, m_sprite_impl(NULL)
	, m_bg(NULL)
{
}

StageCanvas::StageCanvas(StagePanel* panel, wxGLContext* glctx,
						 d2d::ISprite* edited, const d2d::MultiSpritesImpl* bg_sprites)
	: d2d::OrthoCanvas(panel, panel->GetStageImpl(), glctx)
	, m_panel(panel)
	, m_edited(edited)
	, m_sprite_impl(bg_sprites)
	, m_bg(NULL)
{
	m_bg = d2d::draw_all_to_one_spr(m_sprite_impl, m_edited);
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