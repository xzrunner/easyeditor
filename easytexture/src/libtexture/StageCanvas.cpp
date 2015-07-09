#include "StageCanvas.h"
#include "StagePanel.h"
#include "Symbol.h"

namespace etexture
{

StageCanvas::StageCanvas(StagePanel* panel)
	: d2d::OrthoCanvas(panel)
	, m_panel(panel)
	, m_edited(NULL)
{
}

StageCanvas::StageCanvas(StagePanel* panel, d2d::ISprite* edited, 
						 const std::vector<d2d::ISprite*>& bg_sprites)
	: d2d::OrthoCanvas(panel)
	, m_panel(panel)
	, m_edited(edited)
	, m_bg_sprites(bg_sprites)
{
}

void StageCanvas::InitGL()
{
	d2d::OrthoCanvas::InitGL();
	m_panel->GetSymbol()->ReloadTexture();
	if (d2d::Config::Instance()->IsUseDTex()) {
		d2d::DynamicTexAndFont::Instance()->ReloadTexture();
	}
	for (int i = 0, n = m_bg_sprites.size(); i < n; ++i) {
		m_bg_sprites[i]->getSymbol().ReloadTexture();
	}

	ResetViewport();
}

void StageCanvas::OnDrawSprites() const
{
	if (m_edited) 
	{
		d2d::Matrix mat(m_edited->GetTransInvMatrix());
		for (int i = 0, n = m_bg_sprites.size(); i < n; ++i) {
			d2d::SpriteRenderer::Instance()->Draw(m_bg_sprites[i], mat);
		}
	}

	d2d::Rect sr = m_screen.GetRegion();
	m_panel->TraverseSprites(d2d::DrawSpritesVisitor(sr, m_camera->GetScale()), 
		d2d::DT_VISIBLE);
	m_panel->TraverseShapes(d2d::DrawShapesVisitor(sr), d2d::DT_VISIBLE);

	d2d::PrimitiveDraw::cross(d2d::Vector(0, 0), 100, 100, d2d::Colorf(1, 0, 0));

	m_stage->DrawEditOP();
}

}