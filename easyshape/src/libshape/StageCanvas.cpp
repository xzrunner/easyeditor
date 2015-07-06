#include "StageCanvas.h"
#include "StagePanel.h"

namespace libshape
{

StageCanvas::StageCanvas(StagePanel* stage)
	: d2d::DynamicStageCanvas(stage)
	, m_stage_impl(stage)
	, m_edited(NULL)
{
}

StageCanvas::StageCanvas(StagePanel* stage, 
						 d2d::ISprite* edited,
						 const std::vector<d2d::ISprite*>& bg_sprites)
	: d2d::DynamicStageCanvas(stage)
	, m_stage_impl(stage)
	, m_edited(edited)
	, m_bg_sprites(bg_sprites)
{
}

void StageCanvas::drawGuideLines()
{
	d2d::PrimitiveDraw::rect(d2d::Vector(0, 0), 
		d2d::HALF_S_WIDTH,
		d2d::HALF_S_HEIGHT,
		d2d::LIGHT_GREY_LINE);
}

void StageCanvas::initGL()
{
	d2d::DynamicStageCanvas::initGL();

	std::vector<d2d::ISymbol*> symbols;
	d2d::SymbolMgr::Instance()->Traverse(d2d::FetchAllVisitor<d2d::ISymbol>(symbols));
	for (size_t i = 0, n = symbols.size(); i < n; ++i) {
		symbols[i]->ReloadTexture();
	}
}

void StageCanvas::OnDraw()
{
	if (m_edited) 
	{
		d2d::Matrix mat(m_edited->GetTransInvMatrix());
		for (int i = 0, n = m_bg_sprites.size(); i < n; ++i) {
			d2d::SpriteRenderer::Instance()->Draw(m_bg_sprites[i], mat);
		}
	}

	m_stage_impl->traverseShapes(d2d::DrawShapesVisitor(d2d::Rect()), d2d::DT_VISIBLE);

	libshape::StageCanvas::drawGuideLines();

	if (!m_edited) {
		const d2d::ISymbol& symbol = static_cast<StagePanel*>(m_stage)->GetSymbol();
		symbol.Draw(d2d::Matrix());
	}

	m_stage->drawEditTemp();
}

}