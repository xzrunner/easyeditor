#ifndef _EASYANIM_UTILITY_H_
#define _EASYANIM_UTILITY_H_

#include <drag2d.h>

#include "Symbol.h"

namespace libanim
{

class Utility
{
public:
	static void DrawAnimSymbol(
		const Symbol* symbol, 
		const d2d::Matrix& mt = d2d::Matrix(),
		int index = 1, 
		const d2d::ColorTrans& color = d2d::ColorTrans());

	static void GetCurrSprites(const Symbol* symbol, int index, std::vector<d2d::ISprite*>& sprites);

private:
	static Symbol::Frame* GetCurrFrame(Symbol::Layer* layer, int index);
	static Symbol::Frame* GetNextFrame(Symbol::Layer* layer, int index);

}; // Utility

}

#endif // _EASYANIM_UTILITY_H_