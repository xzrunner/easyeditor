#include "StageCanvas.h"
#include "StagePanel.h"
#include "Context.h"
#include "config.h"

#include <ee/DrawSpritesVisitor.h>
#include <ee/EE_RVG.h>
#include <ee/Camera.h>
#include <ee/color_config.h>

namespace etexpacker
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::CameraCanvas(stage, stage->GetStageImpl())
	, m_stage_panel(stage)
	, m_stage_impl(stage)
{
}

void StageCanvas::OnDrawSprites() const
{
	ee::Rect sr = m_screen.GetRegion();
	m_stage_impl->TraverseSprites(ee::DrawSpritesVisitor(sr, m_camera->GetScale()), 
		ee::DT_VISIBLE);

	DrawRegion();
}

void StageCanvas::DrawRegion() const
{
	const float width = Context::Instance()->width,
		height = Context::Instance()->height;

	// bg
	ee::RVG::Color(ee::LIGHT_RED);
	ee::RVG::Rect(ee::Vector(0, 0), ee::Vector(width, height), false);

	int x = 0, y = 0;
	int count = m_stage_panel->GetTextureAccount();
	ee::RVG::Color(ee::LIGHT_GREY);
	for (int i = 0; i < count; ++i)
	{
		ee::RVG::Rect(ee::Vector(x, y), ee::Vector(x + width, y + height), false);
		x += Context::Instance()->width * TEXTURE_X_OFFSET_FACTOR;
	}

	// physics
	m_stage_panel->drawPhysics();
}

}