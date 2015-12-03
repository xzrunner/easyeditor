#include "StageCanvas.h"
#include "StagePanel.h"
#include "OceanMesh.h"

namespace eterrain2d
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

	DrawBG();

	d2d::Rect sr = m_screen.GetRegion();
	m_panel->TraverseSprites(d2d::DrawSpritesVisitor(sr, m_camera->GetScale()), 
		d2d::DT_VISIBLE);
//	m_panel->traverseShapes(d2d::DrawShapesVisitor(sr), d2d::DT_VISIBLE);

	m_stage->DrawEditOP();
}

void StageCanvas::OnTimer()
{
	Update();
}

void StageCanvas::DrawBG() const
{
	const float EDGE = 100;
	d2d::PrimitiveDraw::cross(d2d::Vector(0,0), EDGE, EDGE, d2d::LIGHT_GREY);
}

void StageCanvas::Update()
{
	static clock_t last = 0;
	if (last == 0) {
		last = clock();
		return;
	}

	clock_t curr = clock();
	float dt = (float)(curr - last) / CLOCKS_PER_SEC;
	last = curr;

	std::vector<OceanMesh*>& oceans = m_panel->GetOceans();
	for (int i = 0, n = oceans.size(); i < n; ++i) {
		oceans[i]->Update(dt);
	}
}

}