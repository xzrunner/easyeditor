#include "StageCanvas.h"
#include "StagePanel.h"
#include "GTxt.h"
#include "Sprite.h"

#include <ee/FetchAllVisitor.h>
#include <ee/render_utility.h>
#include <ee/Matrix.h>
#include <ee/SpriteRenderer.h>
#include <ee/Config.h>
#include <ee/EE_DTex.h>
#include <ee/EE_RVG.h>
#include <ee/Math2D.h>

namespace etext
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::OrthoCanvas(stage, stage->GetStageImpl())
	, m_stage(stage)
	, m_edited(NULL)
	, m_sprite_impl(NULL)
	, m_bg(NULL)
{
}

StageCanvas::StageCanvas(StagePanel* stage, wxGLContext* glctx,
						 ee::Sprite* edited, const ee::MultiSpritesImpl* bg_sprites)
	: ee::OrthoCanvas(stage, stage->GetStageImpl(), glctx)
	, m_stage(stage)
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

	ee::SpriteRenderer::Instance()->Draw(m_edited);

	DrawSprBound();

	m_stage->DrawEditOP();

#ifdef _DEBUG 
	if (ee::Config::Instance()->IsUseDTex()) {
		ee::DTex::Instance()->DebugDraw();
	}
#endif
}

void StageCanvas::DrawSprBound() const
{
	ee::Matrix mt;
	m_edited->GetTransMatrix(mt);

	const Sprite* font = static_cast<const Sprite*>(m_edited);
	int hw, hh;
	font->GetSize(hw, hh);
	hw = ee::Math2D::TransLen(hw * 0.5f, mt);
	hh = ee::Math2D::TransLen(hh * 0.5f, mt);

	ee::ShapeStyle style;
	style.filling = false;
	style.color.Set(0.8f, 0.4f, 0.4f);

	ee::RVG::Rect(ee::Vector(0, 0), hw, hh, style);
}

}