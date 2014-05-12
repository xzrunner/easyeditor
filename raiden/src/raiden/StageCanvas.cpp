#include "StageCanvas.h"
#include "StagePanel.h"
#include "Context.h"
#include "ActorInfo.h"

static const d2d::Colorf LIGHT_GRAY = d2d::Colorf(0.8f, 0.8f, 0.8f);

using namespace raiden;

StageCanvas::StageCanvas(StagePanel* parent)
	: d2d::SpriteStageCanvas(parent, parent)
{
}

void StageCanvas::onDraw()
{
	drawGuideLines();

	std::vector<d2d::ISprite*> sprites;
	m_spritesImpl->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites), 
		d2d::e_visible);
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		d2d::ISprite* sprite = sprites[i];
		d2d::SpriteDraw::drawSprite(sprite);

		ActorInfo* info = static_cast<ActorInfo*>(sprite->getUserData());
		if (info->symbol)
			d2d::SpriteDraw::drawSprite(info->symbol, sprite->getPosition() + info->offset);
	}

	m_editPanel->drawEditTemp();
}

void StageCanvas::drawGuideLines()
{
	Context* context = Context::Instance();

	static const float EDGE = 80;
	static const float WIDTH = 480 + EDGE * 2, HEIGHT = 800 * 50;

	// background
	glColor3f(LIGHT_GRAY.r, LIGHT_GRAY.g, LIGHT_GRAY.b);
	glBegin(GL_LINE_LOOP);
		glVertex2f(0, 0);
		glVertex2f(WIDTH, 0);
		glVertex2f(WIDTH, HEIGHT);
		glVertex2f(0, HEIGHT);
	glEnd();
	glBegin(GL_LINES);
		glVertex2f(EDGE, 0); glVertex2f(EDGE, HEIGHT); 
		glVertex2f(WIDTH-EDGE, 0); glVertex2f(WIDTH-EDGE, HEIGHT); 
	glEnd();

	// calibration
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	const float unit = context->speed;
	const int time = HEIGHT / context->speed;
	for (int i = 0; i <= time; ++i)
	{
		if (i % 5 == 0)
		{
			glVertex2f(-EDGE * 3, unit * i);
			glVertex2f(0.0f, unit * i);
		}
		else
		{
			glVertex2f(-EDGE, unit * i);
			glVertex2f(0.0f, unit * i);
		}
	}
	for (int i = 0, n = time / 10; i <= n; ++i)
	{
		if (i % 5 == 0)
		{
			glVertex2f(WIDTH, unit * i * 10);
			glVertex2f(WIDTH + EDGE * 3, unit * i * 10);
		}
		else
		{
			glVertex2f(WIDTH, unit * i * 10);
			glVertex2f(WIDTH + EDGE, unit * i * 10);
		}
	}
	glEnd();
}