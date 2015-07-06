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

	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		d2d::SpriteRenderer::Instance()->Draw(sprites[i], mt, mul, add, r_trans, g_trans, b_trans);

	for_each(sprites.begin(), sprites.end(), DeletePointerFunctor<d2d::ISprite>());
}

void Utility::GetCurrSprites(const Symbol* symbol, int index, std::vector<d2d::ISprite*>& sprites)
{
	for (size_t i = 0, n = symbol->m_layers.size(); i < n; ++i)
	{
		Symbol::Layer* layer = symbol->m_layers[i];

		Symbol::Frame *currFrame = GetCurrFrame(layer, index),
			*nextFrame = GetNextFrame(layer, index);
		if (!currFrame)
			continue;

		if (!nextFrame)
		{
//			;

			for (size_t i = 0, n = currFrame->sprites.size(); i < n; ++i)
				sprites.push_back(currFrame->sprites[i]->Clone());	
		}
		else if (!currFrame->bClassicTween)
		{
			for (size_t i = 0, n = currFrame->sprites.size(); i < n; ++i)
				sprites.push_back(currFrame->sprites[i]->Clone());	
		}
		else
		{
			assert(index >= currFrame->index && index < nextFrame->index);
			float process = (float) (index - currFrame->index) / (nextFrame->index - currFrame->index);
			TweenUtility::GetTweenSprites(currFrame->sprites, nextFrame->sprites, sprites, process);
		}
	}
}

Symbol::Frame* Utility::GetCurrFrame(Symbol::Layer* layer, int index)
{
	Symbol::Frame* ret = NULL;

	for (size_t i = 0, n = layer->frames.size(); i < n; ++i)
	{
		Symbol::Frame* frame = layer->frames[i];
		if (frame->index <= index)
			ret = frame;
		else
			break;
	}
	return ret;
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
