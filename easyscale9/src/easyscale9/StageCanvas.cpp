#include "StageCanvas.h"
#include "StagePanel.h"
#include "ToolbarPanel.h"
#include "ComposeGrids.h"

#include <ee/DrawSpritesVisitor.h>
#include <ee/color_config.h>
#include <ee/cfg_const.h>

#include <easyscale9.h>

#include <sprite2/RenderParams.h>
#include <sprite2/S2_RVG.h>
#include <sprite2/CameraType.h>

namespace escale9
{

StageCanvas::StageCanvas(StagePanel* editPanel)
	: ee::CameraCanvas(editPanel, editPanel->GetStageImpl(), s2::CAM_ORTHO2D)
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
		m_stage_panel->TraverseSprites(ee::DrawSpritesVisitor(GetVisibleRegion(), GetCameraScale()), ee::DT_VISIBLE);
	}
	else
	{
		s2::RVG::SetColor(ee::LIGHT_GREY);
		s2::RVG::Rect(sm::vec2(0, 0), ee::HALF_SCREEN_WIDTH, ee::HALF_SCREEN_HEIGHT, false);

		ee::Symbol* sym = m_stage_panel->getPatchSymbol();
		if (sym)
			sym->Draw(s2::RenderParams());
	}
	m_stage_panel->DrawEditOP();
}

} // escale9