#include "StageCanvas.h"
#include "StagePanel.h"
#include "OceanMesh.h"

namespace eterrain2d
{

BEGIN_EVENT_TABLE(StageCanvas, d2d::OrthoCanvas)
	EVT_TIMER(TIMER_ID, StageCanvas::onTimer)
END_EVENT_TABLE()

StageCanvas::StageCanvas(StagePanel* panel)
	: d2d::OrthoCanvas(panel)
	, m_panel(panel)
	, m_timer(this, TIMER_ID)
	, m_edited(NULL)
{
	m_timer.Start(100);
}

StageCanvas::StageCanvas(StagePanel* panel, d2d::ISprite* edited,
						 const std::vector<d2d::ISprite*>& bg_sprites)
	: d2d::OrthoCanvas(panel)
	, m_panel(panel)
	, m_timer(this, TIMER_ID)
	, m_edited(edited)
	, m_bg_sprites(bg_sprites)
{
	m_timer.Start(100);
}

void StageCanvas::initGL()
{
	d2d::OrthoCanvas::initGL();

	std::vector<d2d::ISymbol*> symbols;
	d2d::SymbolMgr::Instance()->traverse(d2d::FetchAllVisitor<d2d::ISymbol>(symbols));
	for (size_t i = 0, n = symbols.size(); i < n; ++i) {
		symbols[i]->reloadTexture();
	}
}

void StageCanvas::onDraw()
{
	if (m_edited) 
	{
		d2d::Matrix mat(m_edited->GetTransInvMatrix());
		for (int i = 0, n = m_bg_sprites.size(); i < n; ++i) {
			d2d::SpriteDraw::drawSprite(m_bg_sprites[i], mat);
		}
	}

	DrawBG();

	m_panel->traverseSprites(d2d::DrawSpritesVisitor(), d2d::DT_VISIBLE);
//	m_panel->traverseShapes(d2d::DrawShapesVisitor(), d2d::DT_VISIBLE);

	m_editPanel->drawEditTemp();
}

void StageCanvas::onTimer(wxTimerEvent& event)
{
	Update();
	Refresh();
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
		= static_cast<StagePanel*>(m_editPanel)->GetOceans();
	for (int i = 0, n = oceans.size(); i < n; ++i) {
		oceans[i]->Update(dt);
	}
}

}