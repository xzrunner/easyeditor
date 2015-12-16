#include "StageCanvas.h"
#include "StagePanel.h"

namespace libshape
{

StageCanvas::StageCanvas(StagePanel* stage)
	: d2d::OrthoCanvas(stage, stage->GetStageImpl())
	, m_stage(stage)
	, m_shape_impl(stage)
	, m_edited(NULL)
	, m_sprite_impl(NULL)
	, m_bg(NULL)
{
}

StageCanvas::StageCanvas(StagePanel* stage, 
						 wxGLContext* glctx,
						 d2d::ISprite* edited,
						 const d2d::MultiSpritesImpl* bg_sprites)
	: d2d::OrthoCanvas(stage, stage->GetStageImpl(), glctx)
	, m_stage(stage)
	, m_shape_impl(stage)
	, m_edited(edited)
	, m_sprite_impl(bg_sprites)
	, m_bg(NULL)
{
	if (m_sprite_impl) {
		m_bg = d2d::draw_all_to_one_spr(m_sprite_impl, m_edited);
	}
}

void StageCanvas::DrawGuideLines() const
{
	d2d::PrimitiveDraw::rect(d2d::Vector(0, 0), 
		d2d::HALF_S_WIDTH,
		d2d::HALF_S_HEIGHT,
		d2d::LIGHT_GREY_LINE);
}

void StageCanvas::OnDrawSprites() const
{
	if (m_edited && m_bg) 
	{
		d2d::Matrix mat(m_edited->GetTransInvMatrix());
		d2d::SpriteRenderer::Instance()->Draw(m_bg, NULL, mat);
	}

	m_shape_impl->TraverseShapes(d2d::DrawShapesVisitor(d2d::Rect()), d2d::DT_VISIBLE);

	libshape::StageCanvas::DrawGuideLines();

	if (!m_edited) {
		const d2d::ISymbol& symbol = m_stage->GetSymbol();
		symbol.Draw(d2d::Matrix());
	}

	m_stage->DrawEditOP();
}

}