#include "StageCanvas.h"
#include "StagePanel.h"
#include "Context.h"
#include "config.h"

#include <ee/DrawSpritesVisitor.h>
#include <ee/EE_RVG.h>
#include <ee/Camera.h>
#include <ee/style_config.h>

namespace etexpacker
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::OrthoCanvas(stage, stage->GetStageImpl())
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
	ee::RVG::Rect(
		ee::Vector(0, 0), 
		ee::Vector(width, height), 
		ee::LIGHT_RED_LINE);

	int x = 0, y = 0;
	int count = m_stage_panel->GetTextureAccount();
	for (int i = 0; i < count; ++i)
	{
		ee::RVG::Rect(ee::Vector(x, y), 
			ee::Vector(x + width, y + height), 
			ee::LIGHT_GREY_LINE);
		x += Context::Instance()->width * TEXTURE_X_OFFSET_FACTOR;
	}

	// physics
	m_stage_panel->drawPhysics();
}

}