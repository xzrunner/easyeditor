#include "KeyFrame.h"
#include "LayersMgr.h"
#include "Layer.h"
#include "SpriteUserData.h"

#include <ee/ObjectVector.h>

#include <easymesh.h>
#include <easyanim.h>
#include <easyparticle3d.h>

#include <algorithm>

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
	ee::obj_assign(m_layer, layer);
}


void KeyFrame::CopyFromOther(const KeyFrame* src)
{
	if (this == src) {
		return;
	}

	// sprites
	for (size_t i = 0, n = src->m_sprites.size(); i < n; ++i)
	{
		ee::Sprite* s = src->m_sprites[i]->Clone();
		set_sprite_user_data(s, m_layer, this);
		m_sprites.push_back(s);

		if (m_layer) {
			s->SetObserver(&m_layer->GetSpriteObserver());
			m_layer->GetSpriteObserver().Insert(s, m_time);
		}
	}

	// skeleton
	m_skeleton.CopyFrom(m_sprites, src->m_skeleton);
	// todo spr's ud
}

void KeyFrame::Insert(ee::Sprite* sprite, int idx)
{
	sprite->AddReference();

	set_sprite_user_data(sprite, m_layer, this);
	ee::ObjectVector<ee::Sprite>::Insert(m_sprites, sprite, idx);
	if (m_layer) {
		sprite->SetObserver(&m_layer->GetSpriteObserver());
		m_layer->GetSpriteObserver().Insert(sprite, m_time);
	}
}

bool KeyFrame::Remove(ee::Sprite* sprite) 
{
	m_skeleton.RemoveSprite(sprite);
	if (m_layer) {
		m_layer->GetSpriteObserver().Remove(sprite);
	}

	return ee::ObjectVector<ee::Sprite>::Remove(m_sprites, sprite);
}

bool KeyFrame::Reorder(const ee::Sprite* sprite, bool up)
{
	return ee::ObjectVector<ee::Sprite>::ResetOrder(m_sprites, sprite, up);
}

bool KeyFrame::ReorderMost(const ee::Sprite* sprite, bool up)
{
	return ee::ObjectVector<ee::Sprite>::ResetOrderMost(m_sprites, sprite, up);
}

bool KeyFrame::Sort(std::vector<ee::Sprite*>& sprites)
{
	return ee::ObjectVector<ee::Sprite>::Sort(m_sprites, sprites);
}

void KeyFrame::Clear()
{
	if (m_layer) {
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
			m_layer->GetSpriteObserver().Remove(m_sprites[i]);
	}

	ee::ObjectVector<ee::Sprite>::Clear(m_sprites);
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

void KeyFrame::GetTweenSprite(const KeyFrame* start, const KeyFrame* end, 
							  std::vector<ee::Sprite*>& tween, float process)
{
	// 	// old
	// 	for (int i = 0, n = start->Size(); i < n; ++i)
	// 	{
	// 		ee::Sprite* s = start->m_sprites[i];
	// 		for (int j = 0, m = end->Size(); j < m; ++j) {
	// 			ee::Sprite* e = end->m_sprites[j];
	// 			if (IsTweenMatched(s, e))
	// 			{
	// 				ee::Sprite* mid = s->clone();
	// 				GetTweenSprite(s, e, mid, process);
	// 				tween.push_back(mid);
	// 			}
	// 		}
	// 	}

	SkeletonData &s_skeleton = const_cast<KeyFrame*>(start)->GetSkeletonData(),
		&e_skeleton = const_cast<KeyFrame*>(end)->GetSkeletonData();
	if (!s_skeleton.Empty() && !e_skeleton.Empty()) {
		SkeletonData::GetTweenSprites(s_skeleton, e_skeleton, tween, process);
	} else {
		// new
		eanim::TweenUtility::GetTweenSprites(start->m_sprites, end->m_sprites, tween, process);
	}
}

//void KeyFrame::GetTweenSprite(ee::Sprite* start, ee::Sprite* end, 
//							  ee::Sprite* tween, float process) const
//{
//	eanim::TweenUtility::GetTweenSprite(start, end, tween, process);
//
//	emesh::Sprite* s = dynamic_cast<emesh::Sprite*>(start),
//		*mid = dynamic_cast<emesh::Sprite*>(tween),
//		*e = static_cast<emesh::Sprite*>(end);
//	if (s && mid && e) {
//		mid->SetTween(s, e, process);
//	}
//}
//
//bool KeyFrame::IsTweenMatched(const ee::Sprite* s0, const ee::Sprite* s1) const
//{
//	if (eanim::TweenUtility::IsTweenMatched(s0, s1) &&
//		!m_skeleton.IsContainSprite(const_cast<ee::Sprite*>(s0))) {
//		return true;
//	} else {
//		return false;
//	}
//}

} // eanim