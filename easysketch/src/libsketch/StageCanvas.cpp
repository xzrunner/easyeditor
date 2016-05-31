#include "StageCanvas.h"

#include "Symbol.h"
#include "config.h"

#include <ee/EditPanelImpl.h>
#include <ee/FetchAllVisitor.h>
#include <ee/Sprite.h>
#include <ee/SpriteRenderer.h>

namespace esketch
{

StageCanvas::StageCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage, 
						 ee::MultiSpritesImpl* sprites_impl, ee::LibraryPanel* library)
	: e3d::StageCanvas(stage_wnd, stage)
	, m_sprites_impl(sprites_impl)
	, m_library(library)
{
}

sm::ivec2 StageCanvas::TransPos3ProjectToScreen(const sm::vec3& proj) const
{
	sm::vec3 pos = GetCamera3().GetModelViewMat() * proj;
	return e3d::ViewFrustum::TransPos3ProjectToScreen(pos, m_screen_width, m_screen_height);
}

sm::vec3 StageCanvas::TransPos3ScreenToProject(const sm::ivec2& scr, float proj_z) const
{
	sm::vec3 pos = GetCamera3().GetModelViewMat() * sm::vec3(0, 0, proj_z);
	return e3d::ViewFrustum::TransPos3ScreenToProject(scr, pos.z, m_screen_width, m_screen_height);
}

sm::vec3 StageCanvas::TransPos3ScreenToDir(const sm::ivec2& screen) const
{
	return e3d::ViewFrustum::TransPos3ScreenToDir(screen, m_screen_width, m_screen_height);
}

void StageCanvas::OnSize(int w, int h)
{
	e3d::StageCanvas::OnSize(w, h);
	m_screen_width = w;
	m_screen_height = h;
}

void StageCanvas::OnDrawSprites() const
{
	e3d::ShaderMgr::Instance()->SetModelView(GetCamera3().GetModelViewMat());
	DrawBackground();
 	DrawSprites();
 
  	m_stage->DrawEditOP();
}

void StageCanvas::DrawBackground() const
{
	e3d::DrawCross(sm::vec3(0, 0, 0), sm::vec3(10, 10, 10), ee::LIGHT_RED);
//  	e3d::DrawGrids(sm::vec3(-10, -10, 0), sm::vec3(10, 10, 0), sm::vec3(0.5f, 0.5f, FLT_MAX), 
//  		ee::LIGHT_RED);
}

void StageCanvas::DrawSprites() const
{
	std::vector<ee::Sprite*> sprites;
	m_sprites_impl->TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprites));

	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		ee::Sprite* sprite = sprites[i];
		if (!sprite->IsVisible())
			continue;
		ee::SpriteRenderer::Draw(sprite);
	}
}

}