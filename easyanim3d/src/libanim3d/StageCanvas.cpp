#include "StageCanvas.h"
#include "StagePanel.h"

#include <ee/EditPanelImpl.h>
#include <ee/FetchAllVisitor.h>
#include <ee/Sprite.h>
#include <ee/SpriteRenderer.h>

#include <model3/RenderCtxStack.h>
#include <model3/PrimitiveDraw.h>

namespace eanim3d
{

StageCanvas::StageCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage, 
						 ee::MultiSpritesImpl* sprites_impl)
	: e3d::StageCanvas(stage_wnd, stage)
	, m_sprites_impl(sprites_impl)
{
}

void StageCanvas::OnSize(int w, int h)
{
	e3d::StageCanvas::OnSize(w, h);
	m_width = w;
	m_height = h;
}

void StageCanvas::OnDrawSprites() const
{
	auto ctx = m3::RenderCtxStack::Instance()->Top();
	if (!ctx) {
		return;
	}
	const_cast<m3::RenderContext*>(ctx)->SetModelView(GetCamera3().GetModelViewMat());

	DrawBackground();
	DrawSprites();
	m_stage->DrawEditOP();
}

void StageCanvas::DrawBackground() const
{
	m3::PrimitiveDraw::SetColor(0xff0000ff);
	m3::PrimitiveDraw::Cross(sm::vec3(0, 0, 0), sm::vec3(1, 1, 1));
//	m3::PrimitiveDraw::Grids(sm::vec3(-10, -10, 0), sm::vec3(10, 10, 0), sm::vec3(0.5f, 0.5f, FLT_MAX));
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