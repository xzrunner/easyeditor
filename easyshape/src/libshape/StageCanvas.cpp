#include "StageCanvas.h"
#include "StagePanel.h"

namespace libshape
{

BEGIN_EVENT_TABLE(StageCanvas, d2d::OrthoCanvas)
	EVT_MOUSE_EVENTS(StageCanvas::onMouse)
	EVT_KEY_DOWN(StageCanvas::onKeyDown)
END_EVENT_TABLE()

StageCanvas::StageCanvas(StagePanel* stage)
	: d2d::OrthoCanvas(stage)
	, m_stage_impl(stage)
{
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
	m_stage_impl->traverseShapes(d2d::DrawShapesVisitor(), d2d::e_visible);

	libshape::StageCanvas::drawGuideLines();

 	const d2d::ISymbol& symbol = static_cast<StagePanel*>(m_editPanel)->GetSymbol();
	symbol.draw(d2d::Matrix());
}

void StageCanvas::onMouse(wxMouseEvent& event)
{
	m_editPanel->onMouse(event);
}

void StageCanvas::onKeyDown(wxKeyEvent& event)
{
	m_editPanel->onKeyDown(event);
}

}