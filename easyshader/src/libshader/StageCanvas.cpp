#include "StageCanvas.h"
#include "StagePanel.h"

namespace eshader
{

static const int SCREEN_WIDTH = 1024;
static const int SCREEN_HEIGHT = 768;

StageCanvas::StageCanvas(StagePanel* stage)
	: d2d::OrthoCanvas(stage)
{
	m_bgColor.set(1, 1, 1, 1);
}

StageCanvas::~StageCanvas()
{
}

void StageCanvas::onDraw()
{
	DrawBackground();
	DrawSprites();
	m_editPanel->drawEditTemp();
}

void StageCanvas::DrawBackground() const
{
	d2d::PrimitiveDraw::rect(d2d::Matrix(), SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 
		d2d::LIGHT_RED_LINE);
}

void StageCanvas::DrawSprites() const
{
	std::vector<d2d::ISprite*> sprites;
	static_cast<StagePanel*>(m_editPanel)->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		d2d::ISprite* sprite = sprites[i];
		if (!sprite->visiable)
			continue;
		d2d::SpriteDraw::drawSprite(sprites[i]);
	}
}

}