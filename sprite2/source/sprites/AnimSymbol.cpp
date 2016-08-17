#include "AnimSymbol.h"
#include "S2_Sprite.h"
#include "BoundingBox.h"

namespace s2
{

AnimSymbol::AnimSymbol()
{
	
}

void AnimSymbol::Draw(const RenderParams& params, const Sprite* spr) const
{

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
	AnimSymbol::Frame* ret = NULL;
	for (int i = 0, n = frames.size(); i < n; ++i)
	{
		AnimSymbol::Frame* frame = frames[i];
		if (frame->index <= index) {
			ret = frame;
		} else {
			break;
		}
	}
	return ret;
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