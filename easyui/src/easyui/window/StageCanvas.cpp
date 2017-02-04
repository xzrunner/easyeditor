#include "StageCanvas.h"
#include "StagePanel.h"
#include "TidCfg.h"

#include <ee/DrawSpritesVisitor.h>
#include <ee/EE_DTex.h>
#include <ee/Config.h>
#include <ee/color_config.h>

#include <sprite2/RenderParams.h>
#include <sprite2/S2_RVG.h>

#include <easyui.h>

namespace eui
{
namespace window
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::CameraCanvas(stage, stage->GetStageImpl(), s2::CAM_ORTHO2D, NULL, true)
	, m_stage(stage)
{
}

void StageCanvas::OnDrawSprites() const
{
	static bool inited = false;
	if (!inited) {
		m_stage->InitConfig();
		TidCfg::Instance()->LoadConfig();
		inited = true;
	}

	DrawGuideLines();

	m_stage->TraverseSprites(ee::DrawSpritesVisitor(GetVisibleRegion(), GetCameraScale()), ee::DT_VISIBLE);

	m_stage->GetSymbol()->Draw(s2::RenderParams());

	m_stage->DrawEditOP();
}

void StageCanvas::DrawGuideLines() const
{
	int width, height;
	window::QueryWindowViewSizeSJ::Instance()->Query(width, height);

	s2::RVG::SetColor(ee::LIGHT_GREY);
	s2::RVG::Rect(sm::vec2(0, 0), width * 0.5f, height * 0.5f, false);
}

}
}