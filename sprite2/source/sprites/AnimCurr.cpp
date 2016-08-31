#include "AnimCurr.h"
#include "AnimSymbol.h"
#include "AnimLerp.h"
#include "S2_Sprite.h"
#include "DrawNode.h"

#include <assert.h>

namespace s2
{

AnimCurr::AnimCurr()
	: m_sym(NULL)
	, m_frame(0)
	, m_time(0)
{
}

AnimCurr::AnimCurr(const AnimCurr& curr)
	: m_sym(NULL)
	, m_frame(0)
	, m_layers(curr.m_layers)
	, m_time(curr.m_time)
{
	cu::RefCountObjAssign(m_sym, const_cast<AnimSymbol*>(curr.m_sym));
}

AnimCurr& AnimCurr::operator = (const AnimCurr& curr)
{
	m_sym = NULL;
	cu::RefCountObjAssign(m_sym, const_cast<AnimSymbol*>(curr.m_sym));
	m_frame = curr.m_frame;
	m_layers = curr.m_layers;
	m_time = curr.m_time;
	return *this;
}

AnimCurr::AnimCurr(AnimSymbol* sym)
	: m_sym(NULL)
	, m_frame(0)
	, m_time(0)
{
	cu::RefCountObjAssign(m_sym, sym);
}

AnimCurr::~AnimCurr()
{
	if (m_sym) {
		m_sym->RemoveReference();
	}
}

bool AnimCurr::Update(const RenderParams& params, float dt, bool loop,
					  float interval, int fps)
{
	bool dirty = false;

	// update frame
	m_time += dt;
	int curr_frame = m_time * fps;
	int max_frame = m_sym->GetMaxFrameIdx();
	int loop_max_frame = max_frame + interval * fps;
	if (loop) {
		if (curr_frame <= max_frame) {
			++curr_frame;
		} else if (curr_frame > max_frame && curr_frame <= loop_max_frame) {
			curr_frame = 1;
		} else {
			curr_frame = 1;
			m_time = 0;
		}
	} else {
		if (curr_frame > max_frame) {
			curr_frame = max_frame;
		} else {
			++curr_frame;
		}
	}

	// update children
	for (int i = 0, n = m_layers.size(); i < n; ++i) {
		Frame& frame = m_layers[i].frame;
		for (int j = 0, m = frame.sprs.size(); j < m; ++j) {
			if (frame.sprs[j]->Update(params, dt)) {
				dirty = true;
			}
		}
	}

	// update curr frame
	if (curr_frame != m_frame) {
		m_frame = curr_frame;
		dirty = true;
		LoadFromSym();
	}

	return dirty;
}

void AnimCurr::Draw(const RenderParams& params) const
{
	for (int i = 0, n = m_layers.size(); i < n; ++i) {
		const Layer& layer = m_layers[i];
		for (int j = 0, m = layer.frame.sprs.size(); j < m; ++j) {
			DrawNode::Draw(layer.frame.sprs[j], params);
		}
	}
}

void AnimCurr::Start()
{
	m_time = 0;
	m_frame = 1;
	LoadFromSym();
}

void AnimCurr::Clear()
{
	m_frame = 0;
	m_time = 0;
	m_layers.clear();
}

void AnimCurr::LoadFromSym()
{
	if (!m_sym) {
		return;
	}

	const std::vector<AnimSymbol::Layer*>& sym_layers = VI_DOWNCASTING<AnimSymbol*>(m_sym)->GetLayers();
	if (sym_layers.size() != m_layers.size()) 
	{
		for (int i = 0, n = sym_layers.size(); i < n; ++i)
		{
			AnimSymbol::Layer* sym_layer = sym_layers[i];
			AnimSymbol::Frame *curr_f = sym_layer->GetCurrFrame(m_frame),
				              *next_f = sym_layer->GetNextFrame(m_frame);
			Layer layer;
			if (!curr_f) {
				m_layers.push_back(layer);
				continue;
			}
			if (!curr_f->tween || !next_f)
			{
				for (int i = 0, n = curr_f->sprs.size(); i < n; ++i) {
					Sprite* spr = VI_CLONE(Sprite, curr_f->sprs[i]);
					layer.frame.sprs.push_back(spr);	
				}
			}
			else
			{
				assert(m_frame >= curr_f->index && m_frame < next_f->index);
				float process = (float) (m_frame - curr_f->index) / (next_f->index - curr_f->index);
				AnimLerp::Lerp(curr_f->sprs, next_f->sprs, layer.frame.sprs, process);
			}
			m_layers.push_back(layer);
		}
	} 
	else 
	{
		for (int i = 0, n = sym_layers.size(); i < n; ++i)
		{
			AnimSymbol::Layer* sym_layer = sym_layers[i];
			AnimSymbol::Frame *curr_f = sym_layer->GetCurrFrame(m_frame),
				              *next_f = sym_layer->GetNextFrame(m_frame);
			if (!curr_f)
				continue;

			Layer& curr_layer = m_layers[i];
			Frame& old_frame = curr_layer.frame;
			Frame new_frame;
			if (!curr_f->tween || !next_f)
			{
				for (int i = 0, n = curr_f->sprs.size(); i < n; ++i) 
				{
					Sprite* src = curr_f->sprs[i];
					Sprite* dst = old_frame.Query(src);
					if (dst) {
						dst->AddReference();
						*dst = *src;
					} else {
						dst = VI_CLONE(Sprite, src);
					}
					new_frame.sprs.push_back(dst);
				}
			}
			else
			{
				assert(m_frame >= curr_f->index && m_frame < next_f->index);
				float process = (float) (m_frame - curr_f->index) / (next_f->index - curr_f->index);

				for (int curr_idx = 0, curr_n = curr_f->sprs.size(); curr_idx < curr_n; ++curr_idx) 
				{
					Sprite *start_spr = curr_f->sprs[curr_idx], 
						   *end_spr = NULL;				
					for (int next_idx = 0, next_n = next_f->sprs.size(); next_idx < next_n; ++next_idx) 
					{
						Sprite* spr = next_f->sprs[next_idx];
						if (AnimLerp::IsMatched(start_spr, spr))
						{
							end_spr = spr;
							break;
						}
					}
					Sprite* tween_spr = old_frame.Query(start_spr);
					if (tween_spr) {
						tween_spr->AddReference();
					} else {
						tween_spr = VI_CLONE(Sprite, start_spr);
					}
					if (end_spr) {
						AnimLerp::Lerp(start_spr, end_spr, tween_spr, process);
					}
					new_frame.sprs.push_back(tween_spr);
				}
			}

			m_layers[i].frame = new_frame;
		}
	}
}

/************************************************************************/
/* struct AnimCurr::Frame                                               */
/************************************************************************/

AnimCurr::Frame::
Frame()
{
}

AnimCurr::Frame::
Frame(const Frame& f)
{
	CopyFrom(f.sprs);
}

AnimCurr::Frame& AnimCurr::Frame::
operator = (const Frame& f)
{
	CopyFrom(f.sprs);
	return *this;
}

AnimCurr::Frame::~Frame()
{
	for_each(sprs.begin(), sprs.end(), cu::RemoveRefFonctor<Sprite>());
}

Sprite* AnimCurr::Frame::
Query(const Sprite* spr)
{
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		if (sprs[i]->GetSymbol() == spr->GetSymbol() &&
			sprs[i]->GetName() == spr->GetName()) {
			return sprs[i];
		}
	}
	return NULL;
}

void AnimCurr::Frame::
CopyFrom(const std::vector<Sprite*>& src)
{
	for_each(src.begin(), src.end(), cu::AddRefFonctor<Sprite>());
	for_each(sprs.begin(), sprs.end(), cu::RemoveRefFonctor<Sprite>());	
	sprs = src;
}

}