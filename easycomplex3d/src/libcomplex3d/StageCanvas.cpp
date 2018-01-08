#include "StageCanvas.h"

#include <ee/EditPanelImpl.h>
#include <ee/FetchAllVisitor.h>
#include <ee/Sprite.h>
#include <ee/SpriteRenderer.h>
#include <ee/color_config.h>

#include <node3/RenderContext.h>
#include <node3/RenderCtxStack.h>
#include <node3/PrimitiveDraw.h>
#include <node3/Viewport.h>

namespace ecomplex3d
{

StageCanvas::StageCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage, 
						 ee::MultiSpritesImpl* sprites_impl, ee::LibraryPanel* library)
	: e3d::StageCanvas(stage_wnd, stage)
	, m_sprites_impl(sprites_impl)
	, m_library(library)
{
}

void StageCanvas::OnSize(int w, int h)
{
	e3d::StageCanvas::OnSize(w, h);
	m_screen_width = w;
	m_screen_height = h;
}

void StageCanvas::OnDrawSprites() const
{
	auto ctx = n3::RenderCtxStack::Instance()->Top();
	if (!ctx) {
		return;
	}
	const_cast<n3::RenderContext*>(ctx)->SetModelView(
		GetCamera().GetModelViewMat());

	DrawBackground();
 	DrawSprites();
 
  	m_stage->DrawEditOP();
}

void StageCanvas::DrawBackground() const
{
	n3::PrimitiveDraw::SetColor(ee::LIGHT_RED.ToABGR());
	n3::PrimitiveDraw::Cross(sm::vec3(0, 0, 0), sm::vec3(10, 10, 10));
//  	e3d::DrawGrids(sm::vec3(-10, -10, 0), sm::vec3(10, 10, 0), sm::vec3(0.5f, 0.5f, FLT_MAX), 
//  		ee::LIGHT_RED);
}

void StageCanvas::DrawSprites() const
{
	std::vector<ee::SprPtr> sprs;
	m_sprites_impl->TraverseSprites(ee::FetchAllRefVisitor<ee::Sprite>(sprs));

	for (size_t i = 0, n = sprs.size(); i < n; ++i)
	{
		auto& spr = sprs[i];
		if (!spr->IsVisible())
			continue;
		ee::SpriteRenderer::Instance()->Draw(spr.get());
	}
}

}