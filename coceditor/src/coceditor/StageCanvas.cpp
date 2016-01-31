#include "StageCanvas.h"
#include "StagePanel.h"

namespace coceditor
{

static const ee::Colorf LIGHT_GRAY = ee::Colorf(0.8f, 0.8f, 0.8f);
static const int WIDTH = 800;
static const int HEIGHT = 480;

StageCanvas::StageCanvas(StagePanel* editPanel)
	: ee::OrthoCanvas(editPanel)
	, m_batch(100, ee::SpriteBatch::USAGE_STATIC)
{
}

StageCanvas::~StageCanvas()
{
}

void StageCanvas::onDraw()
{
	drawGuideLines();

	StagePanel* editPanel = static_cast<StagePanel*>(m_editPanel);
	editPanel->traverseSprites(ee::DrawSpritesVisitor(m_batch),
		ee::e_visible);
	editPanel->drawEditTemp();
}

void StageCanvas::drawGuideLines()
{
	ee::PrimitiveDraw::drawRect(ee::Vector(0, 0), WIDTH * 0.5f, HEIGHT * 0.5f, 
		false, 2, LIGHT_GRAY);
}
} // coceditor