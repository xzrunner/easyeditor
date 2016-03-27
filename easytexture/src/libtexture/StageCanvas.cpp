#include "StageCanvas.h"
#include "StagePanel.h"
#include "Symbol.h"

#include <ee/FetchAllVisitor.h>
#include <ee/render_utility.h>
#include <ee/Sprite.h>
#include <ee/Matrix.h>
#include <ee/SpriteRenderer.h>
#include <ee/EE_RVG.h>
#include <ee/DrawSpritesVisitor.h>
#include <ee/Camera.h>
#include <ee/DrawShapesVisitor.h>
#include <ee/EditPanelImpl.h>

namespace etexture
{

StageCanvas::StageCanvas(StagePanel* panel)
	: ee::CameraCanvas(panel, panel->GetStageImpl())
	, m_panel(panel)
	, m_edited(NULL)
	, m_sprite_impl(NULL)
	, m_bg(NULL)
{
}

StageCanvas::StageCanvas(StagePanel* panel, wxGLContext* glctx,
						 ee::Sprite* edited, const ee::MultiSpritesImpl* bg_sprites)
	: ee::CameraCanvas(panel, panel->GetStageImpl(), glctx)
	, m_panel(panel)
	, m_edited(edited)
	, m_sprite_impl(bg_sprites)
	, m_bg(NULL)
{
	std::vector<ee::Sprite*> sprites;
	m_sprite_impl->TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprites));
	m_bg = ee::draw_all_to_one_spr(sprites, m_edited);
}

void StageCanvas::OnDrawSprites() const
{
	if (m_edited && m_bg) 
	{
		ee::Matrix mat(m_edited->GetTransInvMatrix());
		ee::SpriteRenderer::Instance()->Draw(m_bg, NULL, mat);
	}

	ee::Rect sr = m_screen.GetRegion();
	m_panel->TraverseSprites(ee::DrawSpritesVisitor(sr, m_camera->GetScale()), 
		ee::DT_VISIBLE);
	m_panel->TraverseShapes(ee::DrawShapesVisitor(sr), ee::DT_VISIBLE);

	ee::RVG::Color(ee::Colorf(1, 0, 0));
	ee::RVG::Cross(ee::Vector(0, 0), 100, 100);

	m_stage->DrawEditOP();
}

}