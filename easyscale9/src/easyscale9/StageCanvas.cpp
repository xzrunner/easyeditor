#include "StageCanvas.h"
#include "StagePanel.h"
#include "ToolbarPanel.h"
#include "ComposeGrids.h"

#include <easyscale9.h>

namespace escale9
{

StageCanvas::StageCanvas(StagePanel* editPanel)
	: d2d::OrthoCanvas(editPanel, editPanel->GetStageImpl())
	, m_stage_panel(editPanel)
	, m_toolbar(NULL)
{
}

StageCanvas::~StageCanvas()
{
}

void StageCanvas::OnDrawSprites() const
{
	if (m_toolbar->isComposeOP())
	{
		ComposeGrids::Draw();
		d2d::Rect sr = m_screen.GetRegion();
		m_stage_panel->TraverseSprites(d2d::DrawSpritesVisitor(sr, m_camera->GetScale()), 
			d2d::DT_VISIBLE);
	}
	else
	{
		d2d::PrimitiveDraw::rect(d2d::Vector(0, 0), 1024 * 0.5f, 768 * 0.5f, d2d::LIGHT_GREY_LINE);

		d2d::ISymbol* symbol = m_stage_panel->getPatchSymbol();
		if (symbol)
			symbol->Draw(d2d::Matrix());
	}
	m_stage_panel->DrawEditOP();
}

} // escale9