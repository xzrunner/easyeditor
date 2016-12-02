#include "KeyFrame.h"
#include "LayersMgr.h"
#include "Layer.h"
#include "SpriteUserData.h"

#include <ee/ObjectVector.h>
#include <ee/Exception.h>

#include <easymesh.h>
#include <easyanim.h>
#include <easyparticle3d.h>

#include <sprite2/AnimLerp.h>
#include <sprite2/ILerp.h>

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
	cu::RefCountObjAssign(m_layer, layer);
}

void KeyFrame::CopyFromOther(const KeyFrame* src)
{
	if (this == src) {
		return;
	}

	// sprs
	for (size_t i = 0, n = src->m_sprs.size(); i < n; ++i)
	{
		ee::Sprite* s = dynamic_cast<ee::Sprite*>(((cu::Cloneable*)src->m_sprs[i])->Clone());
		SpriteUserData::SetSprData(s, src->m_layer, this);
		m_sprs.push_back(s);

		if (src->m_layer) {
			s->SetObserver(&src->m_layer->GetSpriteObserver());
			src->m_layer->GetSpriteObserver().Insert(s, m_time);
		}
	}

	// skeleton
	m_skeleton.CopyFrom(m_sprs, src->m_skeleton);
	// todo spr's ud
}

void KeyFrame::Insert(ee::Sprite* spr, int idx)
{
	if (!spr) {
		throw ee::Exception("KeyFrame::Insert fail: spr null.");
	}
	spr->AddReference();

	SpriteUserData::SetSprData(spr, m_layer, this);
	ee::ObjectVector<ee::Sprite>::Insert(m_sprs, spr, idx);
	if (m_layer) {
		spr->SetObserver(&m_layer->GetSpriteObserver());
		m_layer->GetSpriteObserver().Insert(spr, m_time);
	}
}

bool KeyFrame::Remove(ee::Sprite* spr) 
{
	m_skeleton.RemoveSprite(spr);
	if (m_layer) {
		m_layer->GetSpriteObserver().Remove(spr);
	}

	return ee::ObjectVector<ee::Sprite>::Remove(m_sprs, spr);
}

bool KeyFrame::Reorder(const ee::Sprite* spr, bool up)
{
	return ee::ObjectVector<ee::Sprite>::ResetOrder(m_sprs, spr, up);
}

bool KeyFrame::ReorderMost(const ee::Sprite* spr, bool up)
{
	return ee::ObjectVector<ee::Sprite>::ResetOrderMost(m_sprs, spr, up);
}

bool KeyFrame::Sort(std::vector<ee::Sprite*>& sprs)
{
	return ee::ObjectVector<ee::Sprite>::Sort(m_sprs, sprs);
}

void KeyFrame::Clear()
{
	if (m_layer) {
		for (size_t i = 0, n = m_sprs.size(); i < n; ++i)
			m_layer->GetSpriteObserver().Remove(m_sprs[i]);
	}

	for (int i = 0, n = m_lerps.size(); i < n; ++i) {
		delete m_lerps[i].second;
	}
	m_lerps.clear();

	ee::ObjectVector<ee::Sprite>::Clear(m_sprs);
}

void KeyFrame::OnActive()
{
	for (int i = 0, n = m_sprs.size(); i < n; ++i) {
		if (eparticle3d::Sprite* p3d = dynamic_cast<eparticle3d::Sprite*>(m_sprs[i])) {
			if (p3d->IsAlone()) {
				p3d->OnActive();
			}
		}
	}
}

void KeyFrame::SetLerp(int data, s2::ILerp* lerp)
{
	if (data != s2::AnimLerp::SPR_POS) {
		return;
	}

	assert(m_lerps.size() <= 1);
	if (lerp) {
		if (m_lerps.empty()) {
			m_lerps.push_back(std::make_pair(s2::AnimLerp::SPR_POS, lerp));
		} else {
			assert(m_lerps[0].first == s2::AnimLerp::SPR_POS);
			delete m_lerps[0].second;
			m_lerps[0].second = lerp;
		}
	} else {
		if (!m_lerps.empty()) {
			delete m_lerps[0].second;
			m_lerps.clear();
		}
	}
}

void KeyFrame::GetTweenSprite(const KeyFrame* start, const KeyFrame* end, 
							  std::vector<ee::Sprite*>& tween, float process)
{
	// 	// old
	// 	for (int i = 0, n = start->Size(); i < n; ++i)
	// 	{
	// 		ee::Sprite* s = start->m_sprs[i];
	// 		for (int j = 0, m = end->Size(); j < m; ++j) {
	// 			ee::Sprite* e = end->m_sprs[j];
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
		std::vector<s2::Sprite*> begins, ends, tweens;
		begins.reserve(start->m_sprs.size());
		copy(start->m_sprs.begin(), start->m_sprs.end(), back_inserter(begins));
		ends.reserve(start->m_sprs.size());
		copy(end->m_sprs.begin(), end->m_sprs.end(), back_inserter(ends));

		std::vector<std::pair<s2::AnimLerp::SprData, s2::ILerp*> > lerps;
		lerps.reserve(start->m_lerps.size());
		for (int i = 0, n = start->m_lerps.size(); i < n; ++i) {
			lerps.push_back(std::make_pair(s2::AnimLerp::SprData(start->m_lerps[i].first), start->m_lerps[i].second));
		}
		s2::AnimLerp::Lerp(begins, ends, tweens, process, lerps);

		tween.reserve(tweens.size());
		for (int i = 0, n = tweens.size(); i < n; ++i) {
			tween.push_back(dynamic_cast<ee::Sprite*>(tweens[i]));
		}
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