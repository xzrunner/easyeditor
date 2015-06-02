#include "StageCanvas.h"
#include "StagePanel.h"

namespace eanim
{

StageCanvas::StageCanvas(d2d::EditPanel* stage)
	: d2d::OrthoCanvas(stage)
	, m_background(NULL)
{
}

StageCanvas::~StageCanvas()
{
	if (m_background) {
		m_background->Release();
	}
}

void StageCanvas::SetBackground(d2d::ISymbol* symbol)
{
	d2d::obj_assign((d2d::Object*&)m_background, symbol);
}

void StageCanvas::onDraw()
{
	drawbackground();
	std::vector<d2d::ISprite*> sprites;
	static_cast<StagePanel*>(m_stage)->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites), d2d::DT_VISIBLE);
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		d2d::SpriteDraw::drawSprite(sprites[i]);
	m_stage->drawEditTemp();

#ifdef _DEBUG 
	if (d2d::Config::Instance()->IsUseDTex()) {
		d2d::DynamicTexAndFont::Instance()->DebugDraw();
	}
#endif
}

void StageCanvas::drawbackground() const
{
	if (m_background) {
		d2d::SpriteDraw::drawSprite(m_background);
	}

	float xedge = GetSize().GetWidth() * 0.5f;
	float yedge = GetSize().GetHeight() * 0.5f;
	d2d::PrimitiveDraw::cross(d2d::Vector(0,0), xedge, yedge, d2d::LIGHT_GREY);
}

} // eanim