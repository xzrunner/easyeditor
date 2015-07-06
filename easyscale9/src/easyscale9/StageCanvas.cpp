#include "StageCanvas.h"
#include "StagePanel.h"
#include "ToolbarPanel.h"
#include "config.h"

#include <easyscale9.h>

namespace escale9
{

static const d2d::Colorf LIGHT_GRAY = d2d::Colorf(0.8f, 0.8f, 0.8f);
static const int WIDTH = 800;
static const int HEIGHT = 480;

StageCanvas::StageCanvas(StagePanel* editPanel)
	: d2d::OrthoCanvas(editPanel)
	, m_toolbar(NULL)
{
	m_bgStyle.color = LIGHT_GRAY;
	m_bgStyle.fill = false;
	m_bgStyle.size = 2;

	m_nodeStyle.color = LIGHT_GRAY;
	m_nodeStyle.fill = false;
	m_nodeStyle.size = 2;
}

StageCanvas::~StageCanvas()
{
}

void StageCanvas::OnDraw()
{
	StagePanel* editPanel = static_cast<StagePanel*>(m_stage);
	if (m_toolbar->isComposeOP())
	{
		drawGuideLines();
		d2d::Rect sr = m_screen.GetRegion();
		editPanel->traverseSprites(d2d::DrawSpritesVisitor(sr, m_camera->GetScale()), 
			d2d::DT_VISIBLE);
	}
	else
	{
		d2d::PrimitiveDraw::rect(d2d::Vector(0, 0), 1024 * 0.5f, 768 * 0.5f, m_bgStyle);

		d2d::ISymbol* symbol = editPanel->getPatchSymbol();
		if (symbol)
			symbol->Draw(d2d::Matrix());
	}
	editPanel->drawEditTemp();
}

void StageCanvas::drawGuideLines()
{
	const float edge = EDGE;

	for (size_t i = 0; i < 3; ++i) {
		for (size_t j = 0; j < 3; ++j) {
 			d2d::PrimitiveDraw::rect(d2d::Vector(edge*i, edge*j), 
				d2d::Vector(edge*i+edge, edge*j+edge), m_bgStyle);
		}
	}
}
} // escale9