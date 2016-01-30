#include "KeyFrame.h"
#include "LayersMgr.h"
#include "Layer.h"
#include "SpriteUserData.h"

#include <easymesh.h>
#include <easyanim.h>
#include <easyparticle3d.h>

namespace eanim
{

KeyFrame::KeyFrame(int time)
	: m_layer(NULL)
{
	m_time = time;
	m_classic_tween = false;
	m_id = 0;
}

KeyFrame::~KeyFrame()
{
	Clear();
}

void KeyFrame::SetLayer(Layer* layer) 
{
	d2d::obj_assign(m_layer, layer);
}


void KeyFrame::CopyFromOther(const KeyFrame* src)
{
	if (this == src) {
		return;
	}

	// sprites
	for (size_t i = 0, n = src->m_sprites.size(); i < n; ++i)
	{
		d2d::Sprite* s = src->m_sprites[i]->Clone();
		set_sprite_user_data(s, m_layer, this);
		m_sprites.push_back(s);

		if (m_layer) {
			s->SetObserver(&m_layer->GetSpriteObserver());
			m_layer->GetSpriteObserver().insert(s, m_time);
		}
	}

	// skeleton
	m_skeletonData.CopyFrom(m_sprites, src->m_skeletonData);
	// todo spr's ud
}

void KeyFrame::Insert(d2d::Sprite* sprite)
{
	sprite->Retain();

	set_sprite_user_data(sprite, m_layer, this);
	m_sprites.push_back(sprite);
	if (m_layer) {
		sprite->SetObserver(&m_layer->GetSpriteObserver());
		m_layer->GetSpriteObserver().insert(sprite, m_time);
	}
}

bool KeyFrame::Remove(d2d::Sprite* sprite) 
{
	m_skeletonData.RemoveSprite(sprite);
	if (m_layer) {
		m_layer->GetSpriteObserver().remove(sprite);
	}

	std::vector<d2d::Sprite*>::iterator itr = m_sprites.begin();
	for ( ; itr != m_sprites.end(); ++itr) 
	{
		if (*itr != sprite) {
			continue;
		}

		(*itr)->Release();
		m_sprites.erase(itr);

		return true;
	}

	return false;
}

bool KeyFrame::Reorder(const d2d::Sprite* sprite, bool up)
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
	{
		if (m_sprites[i] != sprite) {
			continue;
		}

		if (up && i != n - 1) {
			std::swap(m_sprites[i], m_sprites[i+1]);
			return true;
		} else if (!up && i != 0) {
			std::swap(m_sprites[i], m_sprites[i-1]);
			return true;
		}

		return false;
	}

	return false;
}

bool KeyFrame::ReorderMost(const d2d::Sprite* sprite, bool up)
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
	{
		if (m_sprites[i] != sprite) {
			continue;
		}

		if (up && i != n - 1) {
			std::swap(m_sprites[i], m_sprites[n - 1]);
			return true;
		} else if (!up && i != 0) {
			std::swap(m_sprites[i], m_sprites[0]);
			return true;
		}

		return false;
	}

	return false;
}

void KeyFrame::Clear()
{
	if (m_layer) {
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
			m_layer->GetSpriteObserver().remove(m_sprites[i]);
	}
	for_each(m_sprites.begin(), m_sprites.end(), d2d::ReleaseObjectFunctor<d2d::Sprite>());
	m_sprites.clear();
}

void KeyFrame::GetTweenSprite(const KeyFrame* start, const KeyFrame* end, 
							   std::vector<d2d::Sprite*>& tween, float process) const
{
// 	// old
// 	for (int i = 0, n = start->Size(); i < n; ++i)
// 	{
// 		d2d::Sprite* s = start->m_sprites[i];
// 		for (int j = 0, m = end->Size(); j < m; ++j) {
// 			d2d::Sprite* e = end->m_sprites[j];
// 			if (IsTweenMatched(s, e))
// 			{
// 				d2d::Sprite* mid = s->clone();
// 				GetTweenSprite(s, e, mid, process);
// 				tween.push_back(mid);
// 			}
// 		}
// 	}

	// new
	libanim::TweenUtility::GetTweenSprites(start->m_sprites, end->m_sprites, tween, process);

  	SkeletonData &s_skeleton = const_cast<KeyFrame*>(start)->GetSkeletonData(),
  		&e_skeleton = const_cast<KeyFrame*>(end)->GetSkeletonData();
	SkeletonData::GetTweenSprites(s_skeleton, e_skeleton, tween, process);
}

void KeyFrame::OnActive()
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		if (eparticle3d::Sprite* p3d = dynamic_cast<eparticle3d::Sprite*>(m_sprites[i])) {
			if (p3d->IsAlone()) {
				p3d->OnActive();
			}
		}
	}
}

void KeyFrame::GetTweenSprite(d2d::Sprite* start, d2d::Sprite* end, 
							  d2d::Sprite* tween, float process) const
{
	libanim::TweenUtility::GetTweenSprite(start, end, tween, process);

	emesh::Sprite* s = dynamic_cast<emesh::Sprite*>(start),
		*mid = dynamic_cast<emesh::Sprite*>(tween),
		*e = static_cast<emesh::Sprite*>(end);
	if (s && mid && e) {
		mid->SetTween(s, e, process);
	}
}

bool KeyFrame::IsTweenMatched(const d2d::Sprite* s0, const d2d::Sprite* s1) const
{
	if (libanim::TweenUtility::IsTweenMatched(s0, s1) &&
		!m_skeletonData.IsContainSprite(const_cast<d2d::Sprite*>(s0))) {
		return true;
	} else {
		return false;
	}
}

} // eanim