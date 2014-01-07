#include "KeyFrame.h"

namespace eanim
{

KeyFrame::KeyFrame(int time)
{
	m_time = time;
	m_bClassicTween = false;
	m_id = 0;
}

KeyFrame::~KeyFrame()
{
	clear();
}

void KeyFrame::copySprites(const KeyFrame* src)
{
	clear();
	for (size_t i = 0, n = src->m_sprites.size(); i < n; ++i)
		m_sprites.push_back(new Sprite(src->m_sprites[i]));
}

void KeyFrame::insert(d2d::ISprite* sprite) 
{
	m_sprites.push_back(new Sprite(sprite));
}

bool KeyFrame::remove(const d2d::ISprite* sprite) 
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i)
	{
		if (m_sprites[i]->curr == sprite) {
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
		if (m_sprites[i]->curr == sprite)
		{
			if (up && i != n - 1)
			{
				Sprite* tmp = m_sprites[i];
				m_sprites[i] = m_sprites[i+1];
				m_sprites[i+1] = tmp;
			}
			else if (!up && i != 0)
			{
				Sprite* tmp = m_sprites[i];
				m_sprites[i] = m_sprites[i-1];
				m_sprites[i-1] = tmp;
			}
		}
	}
}

void KeyFrame::clear()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		delete m_sprites[i];
	m_sprites.clear();
}

void KeyFrame::getTweenSprites(const KeyFrame* start, const KeyFrame* end, std::vector<d2d::ISprite*>& tween, float process)
{
	for (int i = 0, n = start->size(); i < n; ++i)
	{
		Sprite* s = start->m_sprites[i];
		for (int j = 0, m = end->size(); j < m; ++j) {
			Sprite* e = end->m_sprites[j];
			if (s->child == e) {
				d2d::ISprite* mid = s->curr->clone();
				getTweenSprite(s->curr, e->curr, mid, process);
				tween.push_back(mid);
			}
		}
	}
}

void KeyFrame::getTweenSprite(d2d::ISprite* start, d2d::ISprite* end, d2d::ISprite* tween, float process)
{
	d2d::Vector offset = (end->getPosition() - start->getPosition()) * process;
	float delta = (end->getAngle() - start->getAngle()) * process;
	float xscale = (end->getScaleX() - start->getScaleX()) * process + start->getScaleX(),
		yscale = (end->getScaleY() - start->getScaleY()) * process + start->getScaleY();
	float xshear = (end->getShearX() - start->getShearX()) * process + start->getShearX(),
		yshear = (end->getShearY() - start->getShearY()) * process + start->getShearY();
	tween->translate(offset);
	tween->rotate(delta);
	tween->setScale(xscale, yscale);
	tween->setShear(xshear, yshear);
}

} // eanim