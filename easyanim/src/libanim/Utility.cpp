#include "Utility.h"
#include "TweenUtility.h"

namespace libanim
{

void Utility::DrawAnimSymbol(const Symbol* symbol, 
						   const d2d::Matrix& mt,
						   int index, 
						   const d2d::Colorf& mul, 
						   const d2d::Colorf& add,
						   const d2d::Colorf& r_trans,
						   const d2d::Colorf& g_trans,
						   const d2d::Colorf& b_trans)
{
	std::vector<d2d::ISprite*> sprites;
	GetCurrSprites(symbol, index, sprites);

	for (size_t i = 0, n = sprites.size(); i < n; ++i) {
		d2d::SpriteRenderer::Instance()->Draw(sprites[i], mt, mul, add, r_trans, g_trans, b_trans);
	}

	for_each(sprites.begin(), sprites.end(), DeletePointerFunctor<d2d::ISprite>());
}

void Utility::GetCurrSprites(const Symbol* symbol, int index, std::vector<d2d::ISprite*>& sprites)
{
	for (size_t i = 0, n = symbol->m_layers.size(); i < n; ++i)
	{
		Symbol::Layer* layer = symbol->m_layers[i];

		Symbol::Frame *curr_f = GetCurrFrame(layer, index),
			          *next_f = GetNextFrame(layer, index);
		if (!curr_f)
			continue;

		if (!curr_f->bClassicTween || !next_f)
		{
			for (size_t i = 0, n = curr_f->sprites.size(); i < n; ++i) {
				sprites.push_back(curr_f->sprites[i]->Clone());	
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

Symbol::Frame* Utility::GetCurrFrame(Symbol::Layer* layer, int index)
{
	if (layer->frames.empty()) return NULL;

	Symbol::Frame *prev = NULL, *curr = NULL;
	for (size_t i = 0, n = layer->frames.size(); i < n; ++i) {
		Symbol::Frame* frame = layer->frames[i];
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

Symbol::Frame* Utility::GetNextFrame(Symbol::Layer* layer, int index)
{
	for (size_t i = 0, n = layer->frames.size(); i < n; ++i)
	{
		Symbol::Frame* frame = layer->frames[i];
		if (frame->index > index)
			return frame;
	}
	return NULL;
}

} // anim
