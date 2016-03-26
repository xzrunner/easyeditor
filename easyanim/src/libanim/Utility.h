#ifndef _EASYANIM_UTILITY_H_
#define _EASYANIM_UTILITY_H_

#include "Symbol.h"

#include <ee/SpriteTrans.h>

namespace eanim
{

class Utility
{
public:
	static void DrawAnimSymbol(
		const Symbol* symbol, 
		const ee::SpriteTrans& trans = ee::SpriteTrans(),
		int index = 1);

	static void GetCurrSprites(const Symbol* symbol, int index, std::vector<ee::Sprite*>& sprites);

private:
	static Symbol::Frame* GetCurrFrame(Symbol::Layer* layer, int index);
	static Symbol::Frame* GetNextFrame(Symbol::Layer* layer, int index);

}; // Utility

}

#endif // _EASYANIM_UTILITY_H_