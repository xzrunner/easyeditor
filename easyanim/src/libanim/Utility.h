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
		const d2d::Colorf& mul = d2d::Colorf(1,1,1,1), 
		const d2d::Colorf& add = d2d::Colorf(0, 0, 0, 0),
		const d2d::Colorf& r_trans = d2d::Colorf(1, 0, 0, 0),
		const d2d::Colorf& g_trans = d2d::Colorf(0, 1, 0, 0),
		const d2d::Colorf& b_trans = d2d::Colorf(0, 0, 1, 0));

	static void GetCurrSprites(const Symbol* symbol, int index, std::vector<d2d::ISprite*>& sprites);

private:
	static Symbol::Frame* GetCurrFrame(Symbol::Layer* layer, int index);
	static Symbol::Frame* GetNextFrame(Symbol::Layer* layer, int index);

}; // Utility

}

#endif // _EASYANIM_UTILITY_H_