#include "StageCanvas.h"
#include "StagePanel.h"

namespace libshape
{

BEGIN_EVENT_TABLE(StageCanvas, d2d::OrthoCanvas)
	EVT_TIMER(TIMER_ID, StageCanvas::onTimer)
END_EVENT_TABLE()

StageCanvas::StageCanvas(StagePanel* stage)
	: d2d::OrthoCanvas(stage)
	, m_timer(this, TIMER_ID)
	, m_stage_impl(stage)
	, m_edited(NULL)
{
	m_timer.Start(1000 / 30);
}

StageCanvas::StageCanvas(StagePanel* stage, 
						 d2d::ISprite* edited,
						 const std::vector<d2d::ISprite*>& bg_sprites)
	: d2d::OrthoCanvas(stage)
	, m_timer(this, TIMER_ID)
	, m_stage_impl(stage)
	, m_edited(edited)
	, m_bg_sprites(bg_sprites)
{
	m_timer.Start(1000 / 30);
}

void StageCanvas::drawGuideLines()
{
	d2d::PrimitiveDraw::rect(d2d::Vector(0, 0), 
		d2d::HALF_S_WIDTH,
		d2d::HALF_S_HEIGHT,
		d2d::LIGHT_GREY_LINE);
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

	m_stage_impl->traverseShapes(d2d::DrawShapesVisitor(), d2d::DT_VISIBLE);

	libshape::StageCanvas::drawGuideLines();

	if (!m_edited) {
		const d2d::ISymbol& symbol = static_cast<StagePanel*>(m_editPanel)->GetSymbol();
		symbol.draw(d2d::Matrix());
	}

	m_editPanel->drawEditTemp();
}

void StageCanvas::onTimer(wxTimerEvent& event)
{
	Refresh();
}

}