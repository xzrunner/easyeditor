#ifndef _EASYANIM_UTILITY_H_
#define _EASYANIM_UTILITY_H_

#include "Symbol.h"

#include <sprite2/RenderParams.h>

namespace eanim
{

class Utility
{
public:
	static void DrawAnimSymbol(const Symbol* sym, 
		const s2::RenderParams& params = s2::RenderParams(),
		int index = 1);

	static void GetCurrSprites(const Symbol* sym, int index, std::vector<ee::Sprite*>& sprs);

private:
	static s2::AnimSymbol::Frame* GetCurrFrame(s2::AnimSymbol::Layer* layer, int index);
	static s2::AnimSymbol::Frame* GetNextFrame(s2::AnimSymbol::Layer* layer, int index);

}; // Utility

}

#endif // _EASYANIM_UTILITY_H_