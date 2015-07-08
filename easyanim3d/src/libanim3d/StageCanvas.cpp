#include "StageCanvas.h"
#include "StagePanel.h"

namespace eanim3d
{

StageCanvas::StageCanvas(d2d::EditPanel* edit_panel, d2d::MultiSpritesImpl* sprites_impl)
	: e3d::StageCanvas(edit_panel)
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
	m_stage->drawEditTemp();
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