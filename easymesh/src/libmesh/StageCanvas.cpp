#include "StageCanvas.h"
#include "StagePanel.h"
#include "Sprite.h"

#include <ee/SpriteRenderer.h>
#include <ee/EditPanelImpl.h>
#include <ee/DrawShapesVisitor.h>
#include <ee/Config.h>
#include <ee/MultiSpritesImpl.h>
#include <ee/FetchAllVisitor.h>
#include <ee/render_utility.h>

namespace emesh
{

StageCanvas::StageCanvas(StagePanel* panel,
						 wxGLContext* glctx)
	: ee::CameraCanvas(panel, panel->GetStageImpl(), gum::CAM_ORTHO2D, glctx)
	, m_panel(panel)
	, m_edited(NULL)
	, m_sprite_impl(NULL)
	, m_bg(NULL)
{
}

StageCanvas::StageCanvas(StagePanel* panel, wxGLContext* glctx,
						 ee::Sprite* edited, const ee::MultiSpritesImpl* bg_sprites)
	: ee::CameraCanvas(panel, panel->GetStageImpl(), gum::CAM_ORTHO2D, glctx)
	, m_panel(panel)
	, m_edited(edited)
	, m_sprite_impl(bg_sprites)
	, m_bg(NULL)
{
	std::vector<ee::Sprite*> sprs;
	m_sprite_impl->TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprs));
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
		params.mt = m_edited->GetLocalInvMat();
		ee::SpriteRenderer::Instance()->Draw(m_bg, params);
	}

	const ee::Sprite* bg = m_panel->GetBackground();
	if (bg) {
		ee::SpriteRenderer::Instance()->Draw(bg);
	}

	m_stage->DrawEditOP();

	m_panel->TraverseShapes(ee::DrawShapesVisitor(sm::rect(), GetCameraScale()), ee::DT_VISIBLE);
}

}