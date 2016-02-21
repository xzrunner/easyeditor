#include "StageCanvas.h"
#include "StagePanel.h"

#include <ee/Symbol.h>
#include <ee/FetchAllVisitor.h>
#include <ee/SpriteRenderer.h>
#include <ee/SceneNode.h>
#include <ee/EditPanelImpl.h>
#include <ee/PrimitiveDraw.h>
#include <ee/Config.h>
#include <ee/EE_DTex.h>
#include <ee/style_config.h>
#include <ee/Sprite.h>

namespace eanim
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::OrthoCanvas(stage, stage->GetStageImpl())
	, m_stage_panel(stage)
	, m_background(NULL)
{
}

StageCanvas::~StageCanvas()
{
	if (m_background) {
		m_background->Release();
	}
}

void StageCanvas::SetBackground(ee::Symbol* symbol)
{
	ee::obj_assign<ee::Symbol>(m_background, symbol);
}

void StageCanvas::OnDrawSprites() const
{
	DrawBackground();

	std::vector<ee::Sprite*> sprites;
	static_cast<StagePanel*>(m_stage_panel)->TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprites), ee::DT_VISIBLE);
	for (size_t i = 0, n = sprites.size(); i < n; ++i) {
		ee::SpriteRenderer::Instance()->Draw(sprites[i]);
	}

	ee::SceneNodeMgr::Instance()->Draw();

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
	ee::PrimitiveDraw::Cross(ee::Vector(0,0), xedge, yedge, ee::LIGHT_GREY);
}

} // eanim