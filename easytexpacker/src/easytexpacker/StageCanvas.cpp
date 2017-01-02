#include "StageCanvas.h"
#include "StagePanel.h"
#include "Context.h"
#include "config.h"

#include <ee/DrawSpritesVisitor.h>
#include <ee/color_config.h>

#include <sprite2/S2_RVG.h>

namespace etexpacker
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::CameraCanvas(stage, stage->GetStageImpl(), gum::CAM_ORTHO2D)
	, m_stage_panel(stage)
	, m_stage_impl(stage)
{
}

void StageCanvas::OnDrawSprites() const
{
	m_stage_impl->TraverseSprites(ee::DrawSpritesVisitor(GetVisibleRegion(), GetCameraScale()), ee::DT_VISIBLE);

	DrawRegion();
}

void StageCanvas::DrawRegion() const
{
	const float width = Context::Instance()->width,
		height = Context::Instance()->height;

	// bg
	s2::RVG::SetColor(ee::LIGHT_RED);
	s2::RVG::Rect(sm::vec2(0, 0), sm::vec2(width, height), false);

	int x = 0, y = 0;
	int count = m_stage_panel->GetTextureAccount();
	s2::RVG::SetColor(ee::LIGHT_GREY);
	for (int i = 0; i < count; ++i)
	{
		s2::RVG::Rect(sm::vec2(x, y), sm::vec2(x + width, y + height), false);
		x += Context::Instance()->width * TEXTURE_X_OFFSET_FACTOR;
	}

	// physics
	m_stage_panel->drawPhysics();
}

}