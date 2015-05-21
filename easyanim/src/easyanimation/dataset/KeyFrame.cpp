#include "KeyFrame.h"
#include "Layer.h"
#include "SpriteUserData.h"

#include "frame/Controller.h"

#include <easymesh.h>
#include <easyanim.h>

namespace eanim
{

KeyFrame::KeyFrame(Controller* ctrl, int time)
	: m_ctrl(ctrl)
	, m_layer(NULL)
{
	m_layer_idx = m_ctrl->layer();
	m_frame_idx = m_ctrl->frame();

	m_time = time;
	m_bClassicTween = false;
	m_id = 0;
}

KeyFrame::~KeyFrame()
{
	Clear();
}

void KeyFrame::CopyFromOther(const KeyFrame* src)
{
	if (this == src) {
		return;
	}

	// sprites
	for (size_t i = 0, n = src->m_sprites.size(); i < n; ++i)
	{
		d2d::ISprite* s = src->m_sprites[i]->clone();
		set_sprite_user_data(s, m_layer_idx, m_frame_idx);
		m_sprites.push_back(s);

		if (m_layer) {
			s->setObserver(&m_layer->GetSpriteObserver());
			m_layer->GetSpriteObserver().insert(s, m_time);
		}
	}

	// skeleton
	m_skeletonData.copyFrom(m_sprites, src->m_skeletonData);
	// todo spr's ud
}

void KeyFrame::Insert(d2d::ISprite* sprite)
{
	sprite->Retain();

	set_sprite_user_data(sprite, m_layer_idx, m_frame_idx);
	m_sprites.push_back(sprite);
	if (m_layer) {
		sprite->setObserver(&m_layer->GetSpriteObserver());
		m_layer->GetSpriteObserver().insert(sprite, m_time);
	}

	// view list
	KeyFrame* curr = m_ctrl->getCurrFrame();
	if (this == curr) {
		m_ctrl->GetViewlist()->insert(sprite);
	}
}

bool KeyFrame::Remove(d2d::ISprite* sprite) 
{
	m_skeletonData.removeSprite(sprite);
	if (m_layer) {
		m_layer->GetSpriteObserver().remove(sprite);
	}

	std::vector<d2d::ISprite*>::iterator itr = m_sprites.begin();
	for ( ; itr != m_sprites.end(); ++itr) 
	{
		if (*itr != sprite) {
			continue;
		}

		(*itr)->Release();
		m_sprites.erase(itr);

		// viewlist
		KeyFrame* curr = m_ctrl->getCurrFrame();
		if (this == curr) {
			m_ctrl->GetViewlist()->remove(sprite);
		}

		return true;
	}

	return false;
}

void KeyFrame::Reorder(const d2d::ISprite* sprite, bool up)
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
	{
		if (m_sprites[i] != sprite) {
			continue;
		}

		if (up && i != n - 1) {
			std::swap(m_sprites[i], m_sprites[i+1]);
		} else if (!up && i != 0) {
			std::swap(m_sprites[i], m_sprites[i-1]);
		}

		m_ctrl->GetViewlist()->reorder(sprite, up);

		break;
	}
}

void KeyFrame::Clear()
{
	if (m_layer) {
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
			m_layer->GetSpriteObserver().remove(m_sprites[i]);
	}
	for_each(m_sprites.begin(), m_sprites.end(), d2d::ReleaseObjectFunctor<d2d::ISprite>());
	m_sprites.clear();
}

void KeyFrame::getTweenSprites(const KeyFrame* start, const KeyFrame* end, 
							   std::vector<d2d::ISprite*>& tween, float process) const
{
	for (int i = 0, n = start->Size(); i < n; ++i)
	{
		d2d::ISprite* s = start->m_sprites[i];
		for (int j = 0, m = end->Size(); j < m; ++j) {
			d2d::ISprite* e = end->m_sprites[j];
			if (canSpritesTween(*s, *e))
			{
				d2d::ISprite* mid = s->clone();
				getTweenSprite(s, e, mid, process);
				tween.push_back(mid);
			}
		}
	}

  	SkeletonData &s_skeleton = const_cast<KeyFrame*>(start)->getSkeletonData(),
  		&e_skeleton = const_cast<KeyFrame*>(end)->getSkeletonData();
	SkeletonData::getTweenSprites(s_skeleton, e_skeleton, tween, process);
}

void KeyFrame::getTweenSprite(d2d::ISprite* start, d2d::ISprite* end, 
							  d2d::ISprite* tween, float process) const
{
	libanim::Tools::getTweenSprite(start, end, tween, process);

	emesh::Sprite* s = dynamic_cast<emesh::Sprite*>(start),
		*mid = dynamic_cast<emesh::Sprite*>(tween),
		*e = static_cast<emesh::Sprite*>(end);
	if (s && mid && e) {
		mid->SetTween(s, e, process);
	}
}

bool KeyFrame::canSpritesTween(const d2d::ISprite& begin, const d2d::ISprite& end) const
{
	bool autoNamed = false;
	if (!begin.name.empty() && begin.name[0] == '_' && !end.name.empty() && end.name[0] == '_') {
		autoNamed = true;
	}

	if (autoNamed && begin.name == end.name && !m_skeletonData.isContainSprite(const_cast<d2d::ISprite*>(&begin))) {
		return true;
	} else if (begin.name.empty() && end.name.empty()) {
//		return begin.getSymbol().getFilepath() == end.getSymbol().getFilepath();
		return false;
	}
	return false;
}

} // eanim