#include "KeyFrame.h"
#include "Layer.h"

namespace eanim
{

KeyFrame::KeyFrame(int time)
	: m_layer(NULL)
{
	m_time = time;
	m_bClassicTween = false;
	m_id = 0;
}

KeyFrame::~KeyFrame()
{
	clear();
}

void KeyFrame::copyKeyFrame(const KeyFrame* src)
{
//	clear();

	// sprites
	for (size_t i = 0, n = src->m_sprites.size(); i < n; ++i)
	{
		d2d::ISprite* s = src->m_sprites[i]->clone();
		m_sprites.push_back(s);
		if (m_layer) {
			s->setObserver(&m_layer->m_spriteObserver);
			m_layer->m_spriteObserver.insert(s, m_time);
		}
	}

	// skeleton
	m_skeletonData.copyFrom(m_sprites, src->m_skeletonData);
}

void KeyFrame::insert(d2d::ISprite* sprite) 
{
	d2d::ISprite* s = sprite->clone();
	m_sprites.push_back(s);
	if (m_layer) {
		s->setObserver(&m_layer->m_spriteObserver);
		m_layer->m_spriteObserver.insert(s, m_time);
	}
}

bool KeyFrame::remove(d2d::ISprite* sprite) 
{
	m_skeletonData.removeSprite(sprite);
	if (m_layer) {
		m_layer->m_spriteObserver.remove(sprite);
	}
	for (int i = 0, n = m_sprites.size(); i < n; ++i)
	{
		if (m_sprites[i] == sprite) {
			delete m_sprites[i];
			m_sprites.erase(m_sprites.begin() + i);
			return true;
		}
	}
	return false;
}

void KeyFrame::reorder(const d2d::ISprite* sprite, bool up)
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
	{
		if (m_sprites[i] == sprite)
		{
			if (up && i != n - 1)
			{
				d2d::ISprite* tmp = m_sprites[i];
				m_sprites[i] = m_sprites[i+1];
				m_sprites[i+1] = tmp;
			}
			else if (!up && i != 0)
			{
				d2d::ISprite* tmp = m_sprites[i];
				m_sprites[i] = m_sprites[i-1];
				m_sprites[i-1] = tmp;
			}
		}
	}
}

void KeyFrame::clear()
{
	if (m_layer) {
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
			m_layer->m_spriteObserver.remove(m_sprites[i]);
	}
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		delete m_sprites[i];
	m_sprites.clear();
}

void KeyFrame::getTweenSprites(const KeyFrame* start, const KeyFrame* end, std::vector<d2d::ISprite*>& tween, float process)
{
	for (int i = 0, n = start->size(); i < n; ++i)
	{
		d2d::ISprite* s = start->m_sprites[i];
		for (int j = 0, m = end->size(); j < m; ++j) {
			d2d::ISprite* e = end->m_sprites[j];
			if (s->name == e->name) {
				d2d::ISprite* mid = s->clone();
				getTweenSprite(s, e, mid, process);
				tween.push_back(mid);
			}
		}
	}
}

void KeyFrame::getTweenSprite(d2d::ISprite* start, d2d::ISprite* end, d2d::ISprite* tween, float process)
{
	d2d::Vector offset = (end->getPosition() - start->getPosition()) * process;
	float delta = (end->getAngle() - start->getAngle()) * process;
	float xscale = (end->getScale().x - start->getScale().x) * process + start->getScale().x,
		yscale = (end->getScale().y - start->getScale().y) * process + start->getScale().y;
	float xshear = (end->getShear().x - start->getShear().x) * process + start->getShear().x,
		yshear = (end->getShear().y - start->getShear().y) * process + start->getShear().y;
	tween->addCol = cInterpolate(start->addCol, end->addCol, process);
	tween->multiCol = cInterpolate(start->multiCol, end->multiCol, process);
	tween->translate(offset);
	tween->rotate(delta);
	tween->setScale(xscale, yscale);
	tween->setShear(xshear, yshear);
}

} // eanim