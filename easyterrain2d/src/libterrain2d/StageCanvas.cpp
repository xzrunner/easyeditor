#include "StageCanvas.h"
#include "StagePanel.h"
#include "OceanMesh.h"

namespace eterrain2d
{

StageCanvas::StageCanvas(StagePanel* panel)
	: ee::OrthoCanvas(panel, panel->GetStageImpl())
	, m_panel(panel)
	, m_edited(NULL)
	, m_sprite_impl(NULL)
	, m_bg(NULL)
{
}

StageCanvas::StageCanvas(StagePanel* panel, wxGLContext* glctx,
						 ee::Sprite* edited, const ee::MultiSpritesImpl* bg_sprites)
	: ee::OrthoCanvas(panel, panel->GetStageImpl(), glctx)
	, m_panel(panel)
	, m_edited(edited)
	, m_sprite_impl(bg_sprites)
	, m_bg(NULL)
{
	std::vector<Sprite*> sprites;
	m_sprite_impl->TraverseSprites(FetchAllVisitor<Sprite>(sprites));
	m_bg = ee::draw_all_to_one_spr(sprites, m_edited);
}

void StageCanvas::OnDrawSprites() const
{
	if (m_edited && m_bg) 
	{
		ee::Matrix mat(m_edited->GetTransInvMatrix());
		ee::SpriteRenderer::Instance()->Draw(m_bg, NULL, mat);
	}

	DrawBG();

	ee::Rect sr = m_screen.GetRegion();
	m_panel->TraverseSprites(ee::DrawSpritesVisitor(sr, m_camera->GetScale()), 
		ee::DT_VISIBLE);
//	m_panel->traverseShapes(ee::DrawShapesVisitor(sr), ee::DT_VISIBLE);

	m_stage->DrawEditOP();
}

void StageCanvas::OnTimer()
{
	Update();
}

void StageCanvas::DrawBG() const
{
	const float EDGE = 100;
	ee::PrimitiveDraw::Cross(ee::Vector(0,0), EDGE, EDGE, ee::LIGHT_GREY);
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