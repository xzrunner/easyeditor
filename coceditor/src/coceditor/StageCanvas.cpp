#include "StageCanvas.h"
#include "StagePanel.h"

namespace coceditor
{

static const d2d::Colorf LIGHT_GRAY = d2d::Colorf(0.8f, 0.8f, 0.8f);
static const int WIDTH = 800;
static const int HEIGHT = 480;

StageCanvas::StageCanvas(StagePanel* editPanel)
	: d2d::OrthoCanvas(editPanel)
	, m_batch(100, d2d::SpriteBatch::USAGE_STATIC)
{
}

StageCanvas::~StageCanvas()
{
}

void StageCanvas::onDraw()
{
	drawGuideLines();

	StagePanel* editPanel = static_cast<StagePanel*>(m_editPanel);
	editPanel->traverseSprites(d2d::DrawSpritesVisitor(m_batch),
		d2d::e_visible);
	editPanel->drawEditTemp();
}

void StageCanvas::drawGuideLines()
{
	d2d::PrimitiveDraw::drawRect(d2d::Vector(0, 0), WIDTH * 0.5f, HEIGHT * 0.5f, 
		false, 2, LIGHT_GRAY);
}
} // coceditor