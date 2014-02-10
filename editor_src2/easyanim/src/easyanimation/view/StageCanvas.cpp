#include "StageCanvas.h"
#include "StagePanel.h"

#include "dataset/SkeletonData.h"

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

	StagePanel* panel = static_cast<StagePanel*>(m_editPanel);
	panel->getSkeletonData().draw();

	m_editPanel->drawEditTemp();
}

void StageCanvas::drawbackground() const
{
	const float hWidth = GetSize().GetWidth() * 0.5f,
		hHeight = GetSize().GetHeight() * 0.5f;

 	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
 	glBegin(GL_LINES);
		glVertex2f(-hWidth, 0.0f);	glVertex2f(hWidth, 0.0f);
		glVertex2f(0.0f, -hHeight);	glVertex2f(0.0f, hHeight);
 	glEnd();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

} // eanim