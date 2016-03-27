#include "StageCanvas.h"
#include "StagePanel.h"
#include "TidCfg.h"
#include "QueryWindowViewSizeSJ.h"

#include <ee/DrawSpritesVisitor.h>
#include <ee/Camera.h>
#include <ee/EE_DTex.h>
#include <ee/EE_RVG.h>
#include <ee/Config.h>
#include <ee/color_config.h>

namespace eui
{
namespace window
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::CameraCanvas(stage, stage->GetStageImpl(), NULL, false)
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

	ee::Rect sr = m_screen.GetRegion();
	m_stage->TraverseSprites(ee::DrawSpritesVisitor(sr, m_camera->GetScale()), 
		ee::DT_VISIBLE);

	m_stage->GetAnchorMgr()->Draw();

	m_stage->DrawEditOP();

#ifdef _DEBUG 
	if (ee::Config::Instance()->IsUseDTex()) {
		ee::DTex::Instance()->DebugDraw();
	}
#endif
}

void StageCanvas::DrawGuideLines() const
{
	int width, height;
	window::QueryWindowViewSizeSJ::Instance()->Query(width, height);

	ee::RVG::Color(ee::LIGHT_GREY);
	ee::RVG::Rect(ee::Vector(0, 0), width * 0.5f, height * 0.5f, false);
}

}
}