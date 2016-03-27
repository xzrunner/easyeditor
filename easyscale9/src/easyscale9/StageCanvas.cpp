#include "StageCanvas.h"
#include "StagePanel.h"
#include "ToolbarPanel.h"
#include "ComposeGrids.h"

#include <easyscale9.h>

#include <ee/DrawSpritesVisitor.h>
#include <ee/EE_RVG.h>
#include <ee/Camera.h>
#include <ee/Matrix.h>
#include <ee/color_config.h>
#include <ee/SpriteTrans.h>

namespace escale9
{

StageCanvas::StageCanvas(StagePanel* editPanel)
	: ee::CameraCanvas(editPanel, editPanel->GetStageImpl())
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
		ee::RVG::Color(ee::LIGHT_GREY);
		ee::RVG::Rect(ee::Vector(0, 0), 1024 * 0.5f, 768 * 0.5f, false);

		ee::Symbol* symbol = m_stage_panel->getPatchSymbol();
		if (symbol)
			symbol->Draw(ee::SpriteTrans());
	}
	m_stage_panel->DrawEditOP();
}

} // escale9