#include "StageCanvas.h"
#include "StagePanel.h"
#include "Context.h"

static const d2d::Colorf LIGHT_GRAY = d2d::Colorf(0.8f, 0.8f, 0.8f);

using namespace formation;

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
	Context* context = Context::Instance();

	float edge = 80;

	// background
	glColor3f(LIGHT_GRAY.r, LIGHT_GRAY.g, LIGHT_GRAY.b);
	glBegin(GL_LINE_LOOP);
		glVertex2f(0, 0);
		glVertex2f(context->width, 0);
		glVertex2f(context->width, context->height);
		glVertex2f(0, context->height);
	glEnd();

	// calibration
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	const float unit = context->height / context->time;
	for (int i = 0; i <= context->time; ++i)
	{
		if (i % 5 == 0)
		{
			glVertex2f(-edge * 3, unit * i);
			glVertex2f(0.0f, unit * i);
		}
		else
		{
			glVertex2f(-edge, unit * i);
			glVertex2f(0.0f, unit * i);
		}
	}
	for (int i = 0, n = context->time / 10; i <= n; ++i)
	{
		if (i % 5 == 0)
		{
			glVertex2f(context->width, unit * i * 10);
			glVertex2f(context->width + edge * 3, unit * i * 10);
		}
		else
		{
			glVertex2f(context->width, unit * i * 10);
			glVertex2f(context->width + edge, unit * i * 10);
		}
	}
	glEnd();
}