#include "StageCanvas.h"

#include "Symbol.h"
#include "config.h"

namespace libsketch
{

StageCanvas::StageCanvas(wxWindow* stage_wnd, d2d::EditPanelImpl* stage, 
						 d2d::MultiSpritesImpl* sprites_impl, d2d::LibraryPanel* library)
	: e3d::StageCanvas(stage_wnd, stage)
	, m_sprites_impl(sprites_impl)
	, m_library(library)
{
}

ivec2 StageCanvas::TransPos3ProjectToScreen(const vec3& proj) const
{
	vec3 pos = GetCamera3().GetModelViewMat() * proj;
	return e3d::ViewFrustum::TransPos3ProjectToScreen(pos, m_screen_width, m_screen_height);
}

vec3 StageCanvas::TransPos3ScreenToProject(const ivec2& scr, float proj_z) const
{
	vec3 pos = GetCamera3().GetModelViewMat() * vec3(0, 0, proj_z);
	return e3d::ViewFrustum::TransPos3ScreenToProject(scr, pos.z, m_screen_width, m_screen_height);
}

vec3 StageCanvas::TransPos3ScreenToDir(const ivec2& screen) const
{
	return e3d::ViewFrustum::TransPos3ScreenToDir(screen, m_screen_width, m_screen_height);
}

void StageCanvas::InitGL()
{
	e3d::StageCanvas::InitGL();
	m_library->LoadDefaultSymbol();
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
	e3d::DrawCross(vec3(0, 0, 0), vec3(10, 10, 10), d2d::LIGHT_RED);
//  	e3d::DrawGrids(vec3(-10, -10, 0), vec3(10, 10, 0), vec3(0.5f, 0.5f, FLT_MAX), 
//  		d2d::LIGHT_RED);
}

void StageCanvas::DrawSprites() const
{
	std::vector<d2d::ISprite*> sprites;
	m_sprites_impl->TraverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));

	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		d2d::ISprite* sprite = sprites[i];
		if (!sprite->visiable)
			continue;
		d2d::SpriteRenderer::Instance()->Draw(sprite);
	}
}

}