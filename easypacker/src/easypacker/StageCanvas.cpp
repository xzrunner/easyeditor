#include "StageCanvas.h"
#include "StagePanel.h"
#include "Context.h"

static const d2d::Colorf LIGHT_GRAY = d2d::Colorf(0.8f, 0.8f, 0.8f);

using namespace epacker;

StageCanvas::StageCanvas(StagePanel* parent)
	: d2d::SpriteStageCanvas(parent, parent)
{
}

void StageCanvas::onDraw()
{
	drawGuideLines();
	d2d::SpriteStageCanvas::onDraw();
}

void StageCanvas::drawGuideLines()
{
	const float width = Context::Instance()->width,
		height = Context::Instance()->height;

	glColor3f(LIGHT_GRAY.r, LIGHT_GRAY.g, LIGHT_GRAY.b);

	glPushMatrix();
	glBegin(GL_LINES);
	for (size_t i = 0; i <= height; ++i)
	{
		glVertex2f(0, i);
		glVertex2f(width, i);
	}
	for (size_t i = 0; i <= width; ++i)
	{
		glVertex2f(i, 0);
		glVertex2f(i, height);
	}
	glEnd();
	glPopMatrix();
}