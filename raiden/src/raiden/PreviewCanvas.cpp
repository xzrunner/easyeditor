#include "PreviewCanvas.h"
#include "StagePanel.h"

#include "Context.h"
#include "ActorInfo.h"

#include <easyshape.h>

using namespace raiden;

BEGIN_EVENT_TABLE(PreviewCanvas, ee::OrthoCanvas)
	EVT_TIMER(TIMER_ID, PreviewCanvas::onTimer)
END_EVENT_TABLE()

PreviewCanvas::PreviewCanvas(ee::EditPanel* editPanel)
	: ee::OrthoCanvas(editPanel)
	, m_timer(this, TIMER_ID)
	, m_yOffset(0.0f)
{
	m_timer.Start(1000 / FRAME_RATE);
	loadSprites();
}

PreviewCanvas::~PreviewCanvas()
{
	m_timer.Stop();

	for_each(m_sprites.begin(), m_sprites.end(), DeletePointerFunctor<Sprite>());
}

void PreviewCanvas::initGL()
{
	ee::OrthoCanvas::initGL();

	glTranslatef(-(480 + 80*2) * 0.5f, 800 * 0.5f, 0.0f);

	Context::Instance()->library->reloadTexture();
}

void PreviewCanvas::onDraw()
{
	drawSprites();
}

void PreviewCanvas::onTimer(wxTimerEvent& event)
{
	static const float dt = 1.0f / FRAME_RATE;
	static const float OFFSET = Context::Instance()->speed * dt;

	updateSprites(dt);

	m_yOffset += OFFSET;

	glTranslatef(0.0f, -OFFSET, 0.0f);

	Refresh();
}

void PreviewCanvas::loadSprites()
{
	std::vector<ee::ISprite*> src;
	Context::Instance()->stage->traverseSprites(ee::FetchAllVisitor<ee::ISprite>(src), ee::e_visible);
	m_sprites.reserve(src.size());
	for (size_t i = 0, n = src.size(); i < n; ++i)
		m_sprites.push_back(new Sprite(src[i]));
}

void PreviewCanvas::drawSprites()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
	{
		ee::ISprite* sprite = m_sprites[i]->s;

		ee::Vector center = sprite->getBounding()->center();
		float hHeight = sprite->getBounding()->height() * 0.5f;
		if (center.y - m_yOffset < 0+hHeight && center.y - m_yOffset > -800-hHeight)
			m_buffer.insert(m_sprites[i]);
	}

	std::set<Sprite*>::iterator itr = m_buffer.begin();
	for ( ; itr != m_buffer.end(); ++itr)
	{
		Sprite* sprite = *itr;

		if (sprite->finish)
			continue;

		ee::SpriteDraw::drawSprite(sprite->s);
	}
}

void PreviewCanvas::updateSprites(float dt)
{
	std::set<Sprite*>::iterator itr = m_buffer.begin();
	for ( ; itr != m_buffer.end(); ++itr)
	{
		Sprite* sprite = *itr;

		if (sprite->lines.empty())
			continue;

		if (sprite->finish)
			continue;

		//////////////////////////////////////////////////////////////////////////
		sprite->curr += sprite->speed * dt;

		float dis = sprite->curr;
		sprite->finish = true;
		ee::Vector pos;
		for (size_t i = 0, n = sprite->lines.size() - 1; i < n; ++i)
		{
	 		const ee::Vector &start = sprite->lines[i],
	 			&end = sprite->lines[i + 1];
			float seg = ee::Math::getDistance(start, end);
			if (dis < seg)
			{
				sprite->finish = false;
				pos = start + (end - start) * (dis / seg);
				break;
			}
			else
			{
				dis -= seg;
			}
		}

		//////////////////////////////////////////////////////////////////////////

// 		ee::Vector pos;
// 
// 		ee::Vector start = sprite->lines[sprite->index],
// 			end = sprite->lines[sprite->index + 1];
// 
// 		float tot = sprite->speed * dt;
// 		
// 		float curr = sprite->dis + tot;
// 		float seg = ee::Math::getDistance(start, end);
// 		while (true)
// 		{
// 			if (curr < seg)
// 			{
// 				sprite->dis = curr;
// 				pos = start + (end - start) * (curr / seg);
// 				break;
// 			}
// 			else
// 			{
// 				while (sprite->index + 2 < sprite->lines.size())
// 				{
// 					++sprite->index;
// 					sprite->dis = seg - sprite->dis;
// 
// 					start = sprite->lines[sprite->index];
// 					end = sprite->lines[sprite->index+1];
// 					seg = ee::Math::getDistance(start, end);
// 					curr -= seg;
// 				}
// 
// 				if (sprite->index + 2 >= sprite->lines.size())
// 				{
// 					sprite->finish = true;
// 					break;
// 				}
// 			}
// 		}

		//////////////////////////////////////////////////////////////////////////

		sprite->s->setTransform(sprite->base + pos + sprite->offset, sprite->s->getAngle());
	}
}

//////////////////////////////////////////////////////////////////////////
// class PreviewCanvas::Sprite
//////////////////////////////////////////////////////////////////////////

PreviewCanvas::Sprite::Sprite(ee::ISprite* sprite)
{
	s = sprite->clone();

 	ActorInfo* info = static_cast<ActorInfo*>(sprite->getUserData());
 	speed = info->speed;
 
 	const libshape::Symbol* symbol = static_cast<const libshape::Symbol*>(info->symbol);
 	if (symbol && !symbol->shapes.empty())
 	{
		libshape::ChainShape* chain = dynamic_cast<libshape::ChainShape*>(symbol->shapes[0]);
 		if (chain) 
 			lines = chain->getVertices();
 	}
 
 	base = sprite->getPosition();
 	offset = info->offset;
 	curr = 0;

	finish = false;
}