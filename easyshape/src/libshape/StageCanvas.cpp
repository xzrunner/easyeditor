#include "StageCanvas.h"
#include "StagePanel.h"

namespace libshape
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::OrthoCanvas(stage, stage->GetStageImpl())
	, m_stage(stage)
	, m_shape_impl(stage)
	, m_edited(NULL)
	, m_sprite_impl(NULL)
	, m_bg(NULL)
{
}

StageCanvas::StageCanvas(StagePanel* stage, 
						 wxGLContext* glctx,
						 ee::Sprite* edited,
						 const ee::MultiSpritesImpl* bg_sprites)
	: ee::OrthoCanvas(stage, stage->GetStageImpl(), glctx)
	, m_stage(stage)
	, m_shape_impl(stage)
	, m_edited(edited)
	, m_sprite_impl(bg_sprites)
	, m_bg(NULL)
{
	if (m_sprite_impl) {
		std::vector<Sprite*> sprites;
		m_sprite_impl->TraverseSprites(FetchAllVisitor<Sprite>(sprites));
		m_bg = ee::draw_all_to_one_spr(sprites, m_edited);
	}
}

void StageCanvas::DrawGuideLines() const
{
	ee::PrimitiveDraw::DrawRect(ee::Vector(0, 0), 
		ee::HALF_S_WIDTH,
		ee::HALF_S_HEIGHT,
		ee::LIGHT_GREY_LINE);
}

void StageCanvas::OnDrawSprites() const
{
	if (m_edited && m_bg) 
	{
		ee::Matrix mat(m_edited->GetTransInvMatrix());
		ee::SpriteRenderer::Instance()->Draw(m_bg, NULL, mat);
	}

	m_shape_impl->TraverseShapes(ee::DrawShapesVisitor(ee::Rect()), ee::DT_VISIBLE);

	libshape::StageCanvas::DrawGuideLines();

	if (!m_edited) {
		const ee::Symbol& symbol = m_stage->GetSymbol();
		symbol.Draw(ee::Matrix());
	}

	m_stage->DrawEditOP();
}

}