#include "StageCanvas.h"
#include "StagePanel.h"

#include <ee/Symbol.h>
#include <ee/FetchAllVisitor.h>
#include <ee/SpriteRenderer.h>
#include <ee/EditPanelImpl.h>
#include <ee/Config.h>
#include <ee/EE_DTex.h>
#include <ee/color_config.h>
#include <ee/Sprite.h>

#include <sprite2/S2_RVG.h>
#include <sprite2/Particle3d.h>

namespace eanim
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::CameraCanvas(stage, stage->GetStageImpl())
	, m_stage_panel(stage)
	, m_background(NULL)
{
}

StageCanvas::~StageCanvas()
{
	if (m_background) {
		m_background->RemoveReference();
	}
}

void StageCanvas::SetBackground(ee::Symbol* sym)
{
	cu::RefCountObjAssign<ee::Symbol>(m_background, sym);
}

void StageCanvas::OnDrawSprites() const
{
	DrawBackground();

	std::vector<ee::Sprite*> sprs;
	static_cast<StagePanel*>(m_stage_panel)->TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprs), ee::DT_VISIBLE);
	for (size_t i = 0, n = sprs.size(); i < n; ++i) {
		ee::SpriteRenderer::Instance()->Draw(sprs[i]);
	}

	s2::Particle3d::Instance()->Draw();

	m_stage->DrawEditOP();

#ifdef _DEBUG 
	if (ee::Config::Instance()->IsUseDTex()) {
		ee::DTex::Instance()->DebugDraw();
	}
#endif
}

void StageCanvas::DrawBackground() const
{
	if (m_background) {
		ee::SpriteRenderer::Instance()->Draw(m_background);
	}

	float xedge = GetSize().GetWidth() * 0.5f;
	float yedge = GetSize().GetHeight() * 0.5f;
	s2::RVG::SetColor(ee::LIGHT_GREY);
	s2::RVG::Cross(sm::vec2(0,0), xedge, yedge);
}

} // eanim