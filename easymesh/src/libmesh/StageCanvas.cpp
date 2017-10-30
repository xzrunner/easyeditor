#include "StageCanvas.h"
#include "StagePanel.h"
#include "Sprite.h"
#include "Mesh.h"

#include <ee/SpriteRenderer.h>
#include <ee/EditPanelImpl.h>
#include <ee/DrawShapesVisitor.h>
#include <ee/Config.h>
#include <ee/MultiSpritesImpl.h>
#include <ee/FetchAllVisitor.h>
#include <ee/render_utility.h>

#include <sprite2/CameraType.h>

namespace emesh
{

StageCanvas::StageCanvas(StagePanel* panel,
						 wxGLContext* glctx)
	: ee::CameraCanvas(panel, panel->GetStageImpl(), s2::CAM_ORTHO2D, glctx)
	, m_panel(panel)
	, m_edited(NULL)
	, m_sprite_impl(NULL)
	, m_bg(NULL)
{
}

StageCanvas::StageCanvas(StagePanel* panel, wxGLContext* glctx,
						 ee::SprPtr edited, const ee::MultiSpritesImpl* bg_sprites)
	: ee::CameraCanvas(panel, panel->GetStageImpl(), s2::CAM_ORTHO2D, glctx)
	, m_panel(panel)
	, m_edited(edited)
	, m_sprite_impl(bg_sprites)
	, m_bg(NULL)
{
	std::vector<ee::SprPtr> sprs;
	m_sprite_impl->TraverseSprites(ee::FetchAllRefVisitor<ee::Sprite>(sprs));
	m_bg = ee::draw_all_to_one_spr(sprs, m_edited);
}

void StageCanvas::OnDrawSprites() const
{
	if (m_edited && m_bg) 
	{
		s2::RenderParams params;
		params.mt = m_edited->GetLocalMat().Inverted();
		ee::SpriteRenderer::Instance()->Draw(m_bg.get(), params);
	}

	const ee::SprPtr bg = m_panel->GetBackground();
	if (bg) {
		ee::SpriteRenderer::Instance()->Draw(bg.get());
	}

	m_stage->DrawEditOP();

	m_panel->TraverseShapes(ee::DrawShapesVisitor(sm::rect(), GetCameraScale()), ee::DT_VISIBLE);
}

}