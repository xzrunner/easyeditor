
#include "StageCanvas.h"
#include "StagePanel.h"

using namespace libshape;

BEGIN_EVENT_TABLE(StageCanvas, d2d::OrthoCanvas)
	EVT_MOUSE_EVENTS(StageCanvas::onMouse)
	EVT_KEY_DOWN(StageCanvas::onKeyDown)
END_EVENT_TABLE()

static const d2d::Colorf LIGHT_GRAY = d2d::Colorf(0.8f, 0.8f, 0.8f);
static const int WIDTH = 800;
static const int HEIGHT = 480;

StageCanvas::StageCanvas(StagePanel* stage)
	: d2d::ShapeStageCanvas(stage, stage)
{
	m_style.color = LIGHT_GRAY;
}

void StageCanvas::drawGuideLines()
{
	d2d::PrimitiveDraw::rect(d2d::Vector(0, 0), WIDTH * 0.5f, HEIGHT * 0.5f, m_style);
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
		d2d::FileNameParser::isType(symbol.getFilepath(), d2d::FileNameParser::e_shape))
		libshape::StageCanvas::drawGuideLines();
	else
		symbol.draw();

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