#include "StageCanvas.h"
#include "StagePanel.h"
#include "Context.h"
#include "ActorInfo.h"

static const ee::Colorf LIGHT_GRAY = ee::Colorf(0.8f, 0.8f, 0.8f);

using namespace raiden;

StageCanvas::StageCanvas(StagePanel* parent)
	: ee::SpriteStageCanvas(parent, parent)
{
}

void StageCanvas::onDraw()
{
	drawGuideLines();

	std::vector<ee::ISprite*> sprites;
	m_spritesImpl->traverseSprites(ee::FetchAllVisitor<ee::ISprite>(sprites), 
		ee::e_visible);
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		ee::ISprite* sprite = sprites[i];
		ee::SpriteDraw::drawSprite(sprite);

		ActorInfo* info = static_cast<ActorInfo*>(sprite->getUserData());
		if (info->symbol)
			ee::SpriteDraw::drawSprite(info->symbol, sprite->getPosition() + info->offset);
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