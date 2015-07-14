#include "StageCanvas.h"
#include "StagePanel.h"
#include "OceanMesh.h"

namespace eterrain2d
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

	std::vector<d2d::ISymbol*> symbols;
	d2d::SymbolMgr::Instance()->Traverse(d2d::FetchAllVisitor<d2d::ISymbol>(symbols));
	for (size_t i = 0, n = symbols.size(); i < n; ++i) {
		symbols[i]->ReloadTexture();
	}
	if (d2d::Config::Instance()->IsUseDTex()) {
		d2d::DynamicTexAndFont::Instance()->ReloadTexture();
	}
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

	std::vector<OceanMesh*>& oceans 
		= static_cast<StagePanel*>(m_stage)->GetOceans();
	for (int i = 0, n = oceans.size(); i < n; ++i) {
		oceans[i]->Update(dt);
	}
}

}