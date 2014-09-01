#include "StageCanvas.h"
#include "StagePanel.h"

namespace libshape
{

BEGIN_EVENT_TABLE(StageCanvas, d2d::OrthoCanvas)
	EVT_MOUSE_EVENTS(StageCanvas::onMouse)
	EVT_KEY_DOWN(StageCanvas::onKeyDown)
END_EVENT_TABLE()

StageCanvas::StageCanvas(StagePanel* stage)
	: d2d::ShapeStageCanvas(stage, stage)
{
}

void StageCanvas::drawGuideLines()
{
	d2d::PrimitiveDraw::rect(m_screen,
		d2d::Vector(0, 0), 
		d2d::HALF_S_WIDTH,
		d2d::HALF_S_HEIGHT,
		d2d::LIGHT_GREY_LINE);
}

void StageCanvas::initGL()
{
	OrthoCanvas::initGL();

	std::vector<d2d::ISymbol*> symbols;
	d2d::SymbolMgr::Instance()->traverse(d2d::FetchAllVisitor<d2d::ISymbol>(symbols));
	for (size_t i = 0, n = symbols.size(); i < n; ++i)
	{
		symbols[i]->reloadTexture();
	}
}

void StageCanvas::onDraw()
{
 	const d2d::ISymbol& symbol = static_cast<StagePanel*>(m_editPanel)->getSymbol();

	if (symbol.getFilepath().empty() || 
		d2d::FileNameParser::isType(symbol.getFilepath(), d2d::FileNameParser::e_shape)) {
		libshape::StageCanvas::drawGuideLines();
	} else {
		symbol.draw(m_screen, d2d::Matrix());
	}

	d2d::ShapeStageCanvas::onDraw();
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