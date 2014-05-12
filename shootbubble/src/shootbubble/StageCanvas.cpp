#include "StageCanvas.h"
#include "StagePanel.h"
#include "Context.h"

static const d2d::Colorf LIGHT_GRAY = d2d::Colorf(0.8f, 0.8f, 0.8f);

using namespace shootbubble;

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

	glPushMatrix();

	float hEdge, posOffset, posOffset1, rowOffset;
	static_cast<StagePanel*>(m_editPanel)->computeParams(hEdge, posOffset, posOffset1, rowOffset);

	glColor3f(LIGHT_GRAY.r, LIGHT_GRAY.g, LIGHT_GRAY.b);
	d2d::Vector base(hEdge, -posOffset1);
	for (size_t i = 0; i < context->row; ++i)
	{
		int col = (i % 2 ? context->col - 1 : context->col);
		for (size_t j = 0; j < col; ++j)
		{
			glBegin(GL_LINE_LOOP);
				glVertex2f(base.x - hEdge, base.y + posOffset);
				glVertex2f(base.x		 , base.y + posOffset1);
				glVertex2f(base.x + hEdge, base.y + posOffset);
				glVertex2f(base.x + hEdge, base.y - posOffset);
				glVertex2f(base.x		 , base.y - posOffset1);
				glVertex2f(base.x - hEdge, base.y - posOffset);
			glEnd();
			base.x += context->edge;
		}
		base.x = (i % 2 ? hEdge: context->edge);
		base.y -= rowOffset;
	}

	int width = context->edge * context->col,
	height = posOffset1 + rowOffset * (context->row - 1) + posOffset1;

	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(width, 0.0f);
		glVertex2f(width, -height);
		glVertex2f(0.0f, -height);
	glEnd();

	glPopMatrix();
}