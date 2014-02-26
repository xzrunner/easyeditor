#include "StageCanvas.h"
#include "StagePanel.h"

namespace eanim
{

StageCanvas::StageCanvas(d2d::EditPanel* stage)
	: d2d::OrthoCanvas(stage)
{
}

void StageCanvas::onDraw()
{
	drawbackground();
	std::vector<d2d::ISprite*> sprites;
	static_cast<StagePanel*>(m_editPanel)->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		d2d::SpriteDraw::drawSprite(sprites[i]);
	m_editPanel->drawEditTemp();
}

void StageCanvas::drawbackground() const
{
	float xedge = GetSize().GetWidth() * 0.5f;
	float yedge = GetSize().GetHeight() * 0.5f;
	d2d::PrimitiveDraw::cross(d2d::Vector(0,0), xedge, yedge,
		d2d::Colorf(0.8f,0.8f,0.8f,0.8f));
}

} // eanim