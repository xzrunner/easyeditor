#include "AnimSprite.h"
#include "AnimSymbol.h"
#include "AnimLerp.h"
#include "DrawNode.h"

#include <assert.h>

namespace s2
{

AnimSprite::AnimSprite() 
	: m_time(0)
	, m_loop(true)
{
}

AnimSprite::AnimSprite(Symbol* sym) 
	: Sprite(sym)
	, m_time(0)
	, m_loop(true)
{
	Start();
}

AnimSprite* AnimSprite::Clone() const
{
	return new AnimSprite(*this);
}

bool AnimSprite::Update(const RenderParams& params, float dt)
{
	bool dirty = false;

	// update children
	for (int i = 0, n = m_curr_frame.layers.size(); i < n; ++i) {
		Frame& frame = m_curr_frame.layers[i].frame;
		for (int j = 0, m = frame.sprs.size(); j < m; ++j) {
			if (frame.sprs[j]->Update(params, dt)) {
				dirty = true;
			}
		}
	}

	// update curr frame
	m_time += dt;
	AnimSymbol* sym = VI_DOWNCASTING<AnimSymbol*>(m_sym);
	int curr_frame = m_time * sym->GetFPS();
	int max_frame = sym->GetMaxFrameIdx();
	if (!m_loop && curr_frame > max_frame) {
		curr_frame = 1;
	} else {
		curr_frame = (curr_frame % max_frame) + 1;
	}
	if (curr_frame != m_curr_frame.frame) {
		m_curr_frame.frame = curr_frame;
		dirty = true;
		UpdateCurrFrame();
	}

	return dirty;
}

void AnimSprite::Start()
{
	m_time = 0;
	m_curr_frame.frame = 1;
	UpdateCurrFrame();
}

void AnimSprite::Draw(const RenderParams& params) const
{
	for (int i = 0, n = m_curr_frame.layers.size(); i < n; ++i) {
		const Layer& layer = m_curr_frame.layers[i];
		for (int j = 0, m = layer.frame.sprs.size(); j < m; ++j) {
			DrawNode::Draw(layer.frame.sprs[j], params);
		}
	}
}

void AnimSprite::UpdateCurrFrame()
{
	const std::vector<AnimSymbol::Layer*>& sym_layers = VI_DOWNCASTING<AnimSymbol*>(m_sym)->GetLayers();
	if (sym_layers.size() != m_curr_frame.layers.size()) 
	{
		for (int i = 0, n = sym_layers.size(); i < n; ++i)
		{
			AnimSymbol::Layer* sym_layer = sym_layers[i];
			AnimSymbol::Frame *curr_f = sym_layer->GetCurrFrame(m_curr_frame.frame),
				              *next_f = sym_layer->GetNextFrame(m_curr_frame.frame);
			Layer layer;
			if (!curr_f) {
				m_curr_frame.layers.push_back(layer);
				continue;
			}
			if (!curr_f->tween || !next_f)
			{
				for (int i = 0, n = curr_f->sprs.size(); i < n; ++i) {
					Sprite* spr = dynamic_cast<Sprite*>(((cu::Cloneable*)curr_f->sprs[i])->Clone());
					layer.frame.sprs.push_back(spr);	
				}
			}
			else
			{
				assert(m_curr_frame.frame >= curr_f->index && m_curr_frame.frame < next_f->index);
				float process = (float) (m_curr_frame.frame - curr_f->index) / (next_f->index - curr_f->index);
				AnimLerp::Lerp(curr_f->sprs, next_f->sprs, layer.frame.sprs, process);
			}
			m_curr_frame.layers.push_back(layer);
		}
	} 
	else 
	{
		for (int i = 0, n = sym_layers.size(); i < n; ++i)
		{
			AnimSymbol::Layer* sym_layer = sym_layers[i];
			AnimSymbol::Frame *curr_f = sym_layer->GetCurrFrame(m_curr_frame.frame),
				              *next_f = sym_layer->GetNextFrame(m_curr_frame.frame);
			if (!curr_f)
				continue;

			Layer& curr_layer = m_curr_frame.layers[i];
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
						dst = dynamic_cast<Sprite*>(((cu::Cloneable*)src)->Clone());
					}
					new_frame.sprs.push_back(dst);
				}
			}
			else
			{
				assert(m_curr_frame.frame >= curr_f->index && m_curr_frame.frame < next_f->index);
				float process = (float) (m_curr_frame.frame - curr_f->index) / (next_f->index - curr_f->index);

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
						tween_spr = VI_DOWNCASTING<Sprite*>(((cu::Cloneable*)start_spr)->Clone());
					}
					if (end_spr) {
						AnimLerp::Lerp(start_spr, end_spr, tween_spr, process);
					}
					new_frame.sprs.push_back(tween_spr);
				}
			}

			for_each(old_frame.sprs.begin(), old_frame.sprs.end(), cu::RemoveRefFonctor<Sprite>());
			m_curr_frame.layers[i].frame = new_frame;
		}
	}
}

/************************************************************************/
/* struct AnimSprite::Frame                                             */
/************************************************************************/

Sprite* AnimSprite::Frame::
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

}