#include "AnimSymbol.h"
#include "AnimSprite.h"
#include "S2_Sprite.h"
#include "BoundingBox.h"
#include "RenderParams.h"
#include "AnimLerp.h"

#include <assert.h>

namespace s2
{

AnimSymbol::AnimSymbol()
	: m_fps(30)
	, m_curr(this)
{
}

bool AnimSymbol::Update(const RenderParams& params, float dt)
{
	return m_curr.Update(params, dt);
}

void AnimSymbol::Draw(const RenderParams& params, const Sprite* spr) const
{	
	if (spr) {
		RenderParams p = params;
		p.mt = spr->GetTransMatrix() * params.mt;
		p.color = spr->Color() * params.color;
		VI_DOWNCASTING<const AnimSprite*>(spr)->GetAnimCurr().Draw(p);
	} else {
		m_curr.Draw(params);
	}
}

sm::rect AnimSymbol::GetBounding(const Sprite* spr) const
{
	sm::rect b;
	b.MakeEmpty();
	for (int i = 0, n = m_layers.size(); i < n; ++i) {
		Layer* layer = m_layers[i];
		for (int j = 0, m = layer->frames.size(); j < m; ++j) {
			Frame* frame = layer->frames[j];
			for (int k = 0, l = frame->sprs.size(); k < l; ++k) {
				frame->sprs[k]->GetBounding()->CombineTo(b);
			}
		}
	}
	return b;
}

int AnimSymbol::GetMaxFrameIdx() const
{
	int index = 0;
	for (int i = 0, n = m_layers.size(); i < n; ++i)
	{
		AnimSymbol::Layer* layer = m_layers[i];
		for (int j = 0, m = layer->frames.size(); j < m; ++j) {
			index = std::max(index, layer->frames[j]->index);
		}
	}
	return index;
}

void AnimSymbol::CreateFrameSprites(int frame, std::vector<Sprite*>& sprs) const
{
	for (int i = 0, n = m_layers.size(); i < n; ++i)
	{
		Layer* layer = m_layers[i];
		Frame *curr_f = layer->GetCurrFrame(frame),
			  *next_f = layer->GetNextFrame(frame);
		if (!curr_f)
			continue;
		if (!curr_f->tween || !next_f)
		{
			for (int i = 0, n = curr_f->sprs.size(); i < n; ++i) {
				Sprite* spr = VI_CLONE(Sprite, curr_f->sprs[i]);
				sprs.push_back(spr);	
			}
		}
		else
		{
			assert(frame >= curr_f->index && frame < next_f->index);
			float process = (float) (frame - curr_f->index) / (next_f->index - curr_f->index);
			AnimLerp::Lerp(curr_f->sprs, next_f->sprs, sprs, process);
		}
	}
}

void AnimSymbol::AddLayer(Layer* layer)
{
	m_layers.push_back(layer);
}

bool AnimSymbol::Clear()
{
	bool dirty = false;
 	for (int i = 0, n = m_layers.size(); i < n; ++i) {
 		Layer* layer = m_layers[i];
 		for (int j = 0, m = layer->frames.size(); j < m; ++j) {
 			Frame* frame = layer->frames[j];
 			for (int k = 0, l = frame->sprs.size(); k < l; ++k) {
 				frame->sprs[k]->RemoveReference();
				dirty = true;
 			}
			delete frame;
 		}
		delete layer;
 	}
	return dirty;	
}

/************************************************************************/
/* class AnimSymbol::Layer                                              */
/************************************************************************/

AnimSymbol::Frame* AnimSymbol::Layer::GetCurrFrame(int index) const
{
	if (frames.empty()) {
		return NULL;
	}

	AnimSymbol::Frame *prev = NULL, *curr = NULL;
	for (int i = 0, n = frames.size(); i < n; ++i)
	{
		AnimSymbol::Frame* frame = frames[i];
		if (frame->index >= index) {
			curr = frame;
			break;
		} else {
			prev = frame;
		}
	}
	
	if (!curr) {
		return NULL;
	} else if (curr->index == index) {
		return curr;
	} else if (!prev) {
		return NULL;
	} else {
		assert(prev->index <= index);
		return prev;
	}
}

AnimSymbol::Frame* AnimSymbol::Layer::GetNextFrame(int index) const
{
	for (int i = 0, n = frames.size(); i < n; ++i)
	{
		AnimSymbol::Frame* frame = frames[i];
		if (frame->index > index) {
			return frame;
		}
	}
	return NULL;
}

}