#include "Utility.h"
#include "TweenUtility.h"

#include <ee/SpriteRenderer.h>
#include <ee/std_functor.h>
#include <ee/Sprite.h>

#include <sprite2/S2_Sprite.h>

#include <algorithm>

#include <assert.h>

namespace eanim
{

void Utility::DrawAnimSymbol(const Symbol* symbol, 
							 const s2::RenderParams& params,
							 int index)
{
	std::vector<ee::Sprite*> sprites;
	GetCurrSprites(symbol, index, sprites);

	for (size_t i = 0, n = sprites.size(); i < n; ++i) {
		ee::SpriteRenderer::Draw(sprites[i], params);
	}

	for_each(sprites.begin(), sprites.end(), ee::DeletePointerFunctor<ee::Sprite>());
}

void Utility::GetCurrSprites(const Symbol* symbol, int index, std::vector<ee::Sprite*>& sprites)
{
	const std::vector<s2::AnimSymbol::Layer*>& layers = symbol->GetLayers();
	for (size_t i = 0, n = layers.size(); i < n; ++i)
	{
		s2::AnimSymbol::Layer* layer = layers[i];

		s2::AnimSymbol::Frame *curr_f = GetCurrFrame(layer, index),
			                  *next_f = GetNextFrame(layer, index);
		if (!curr_f)
			continue;

		if (!curr_f->tween || !next_f)
		{
			for (size_t i = 0, n = curr_f->sprites.size(); i < n; ++i) {
				ee::Sprite* spr = static_cast<ee::Sprite*>(curr_f->sprites[i]->GetUD());
				sprites.push_back(spr->Clone());	
			}
		}
		else
		{
			assert(index >= curr_f->index && index < next_f->index);
			float process = (float) (index - curr_f->index) / (next_f->index - curr_f->index);
			TweenUtility::GetTweenSprites(curr_f->sprites, next_f->sprites, sprites, process);
		}
	}
}

s2::AnimSymbol::Frame* Utility::GetCurrFrame(s2::AnimSymbol::Layer* layer, int index)
{
	if (layer->frames.empty()) return NULL;

	s2::AnimSymbol::Frame *prev = NULL, *curr = NULL;
	for (size_t i = 0, n = layer->frames.size(); i < n; ++i) {
		s2::AnimSymbol::Frame* frame = layer->frames[i];
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

s2::AnimSymbol::Frame* Utility::GetNextFrame(s2::AnimSymbol::Layer* layer, int index)
{
	for (size_t i = 0, n = layer->frames.size(); i < n; ++i)
	{
		s2::AnimSymbol::Frame* frame = layer->frames[i];
		if (frame->index > index)
			return frame;
	}
	return NULL;
}

} // anim
