#include "StageCanvas.h"
#include "StagePanel.h"
#include "ToolbarPanel.h"
#include "ComposeGrids.h"

#include <easyscale9.h>

#include <ee/DrawSpritesVisitor.h>
#include <ee/PrimitiveDraw.h>
#include <ee/Camera.h>
#include <ee/Matrix.h>
#include <ee/style_config.h>

namespace escale9
{

StageCanvas::StageCanvas(StagePanel* editPanel)
	: ee::OrthoCanvas(editPanel, editPanel->GetStageImpl())
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
		ee::Rect sr = m_screen.GetRegion();
		m_stage_panel->TraverseSprites(ee::DrawSpritesVisitor(sr, m_camera->GetScale()), 
			ee::DT_VISIBLE);
	}
	else
	{
		ee::PrimitiveDraw::DrawRect(ee::Vector(0, 0), 1024 * 0.5f, 768 * 0.5f, ee::LIGHT_GREY_LINE);

		ee::Symbol* symbol = m_stage_panel->getPatchSymbol();
		if (symbol)
			symbol->Draw(ee::Matrix());
	}
	m_stage_panel->DrawEditOP();
}

} // escale9