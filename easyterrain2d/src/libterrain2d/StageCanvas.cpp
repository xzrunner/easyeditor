#include "StageCanvas.h"
#include "StagePanel.h"
#include "OceanMesh.h"

#include <ee/FetchAllVisitor.h>
#include <ee/render_utility.h>
#include <ee/SpriteRenderer.h>
#include <ee/DrawSpritesVisitor.h>
#include <ee/color_config.h>
#include <ee/EditPanelImpl.h>

#include <sprite2/RVG.h>
#include <sprite2/CameraType.h>

namespace eterrain2d
{

StageCanvas::StageCanvas(StagePanel* panel)
	: ee::CameraCanvas(panel, panel->GetStageImpl(), s2::CAM_ORTHO2D)
	, m_panel(panel)
	, m_edited(NULL)
	, m_sprite_impl(NULL)
	, m_bg(NULL)
{
}

StageCanvas::StageCanvas(StagePanel* panel, wxGLContext* glctx,
						 ee::SprPtr edited, const ee::MultiSpritesImpl* bg_sprites)
	: ee::CameraCanvas(panel, panel->GetStageImpl(), s2::CAM_ORTHO2D)
	, m_panel(panel)
	, m_edited(edited)
	, m_sprite_impl(bg_sprites)
	, m_bg(NULL)
{
	std::vector<ee::SprPtr> sprs;
	m_sprite_impl->TraverseSprites(ee::FetchAllRefVisitor<ee::Sprite>(sprs));
	m_bg = ee::draw_all_to_one_spr(sprs, m_edited);
}

StageCanvas::~StageCanvas()
{
	if (m_bg) {
		m_bg->RemoveReference();
	}
}

void StageCanvas::OnDrawSprites() const
{
	if (m_edited && m_bg) 
	{
		s2::RenderParams params;
		params.mt = m_edited->GetLocalMat().Inverted();
		ee::SpriteRenderer::Instance()->Draw(m_bg, params);
	}

	DrawBG();

	m_panel->TraverseSprites(ee::DrawSpritesVisitor(GetVisibleRegion(), GetCameraScale()), ee::DT_VISIBLE);
//	m_panel->traverseShapes(ee::DrawShapesVisitor(sr), ee::DT_VISIBLE);

	m_stage->DrawEditOP();
}

void StageCanvas::OnTimer()
{
	Update();
}

void StageCanvas::DrawBG() const
{
	s2::RVG::SetColor(ee::LIGHT_GREY);
	const float EDGE = 100;
	s2::RVG::Cross(sm::vec2(0,0), EDGE, EDGE);
}

void StageCanvas::Update()
{
	static clock_t last = 0;
	if (last == 0) {
		last = clock();
		return;
	}

	clock_t curr = clock();
	float dt = (float)(curr - last) / CLOCKS_PER_SEC;
	last = curr;

	std::vector<OceanMesh*>& oceans = m_panel->GetOceans();
	for (int i = 0, n = oceans.size(); i < n; ++i) {
		oceans[i]->Update(dt);
	}
}

}