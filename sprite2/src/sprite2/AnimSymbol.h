#ifndef _SPRITE2_ANIM_SYMBOL_H_
#define _SPRITE2_ANIM_SYMBOL_H_

#include "Symbol.h"

#include <vector>

namespace s2
{

class AnimSymbol : public Symbol
{
public:
	AnimSymbol();

	virtual void Draw(const RenderParams& params, const Sprite* spr = NULL) const;

private:
	struct Frame
	{
		int index;
		std::vector<s2::Sprite*> sprites;
		bool tween;
	};

	struct Layer
	{
		std::vector<Frame*> frames;
	};

private:
	std::vector<Layer*> m_layers;

}; // AnimSymbol

}

#endif // _SPRITE2_ANIM_SYMBOL_H_