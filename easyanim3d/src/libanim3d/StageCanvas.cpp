#include "StageCanvas.h"
#include "StagePanel.h"

#include <ee/EditPanelImpl.h>
#include <ee/FetchAllVisitor.h>
#include <ee/Sprite.h>
#include <ee/SpriteRenderer.h>

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
	std::vector<ee::Sprite*> sprs;
	m_sprites_impl->TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprs));

	for (size_t i = 0, n = sprs.size(); i < n; ++i)
	{
		ee::Sprite* spr = sprs[i];
		if (!spr->IsVisible())
			continue;
		ee::SpriteRenderer::Draw(spr);
	}
}

}