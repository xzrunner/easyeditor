#pragma once

#include <drag2d.h>

#include "Symbol.h"

namespace libanim
{

class Tools
{
public:
	static void drawAnimSymbol(
		const Symbol* symbol, 
		const d2d::Matrix& mt = d2d::Matrix(),
		int index = 1, 
		const d2d::Colorf& mul = d2d::Colorf(1,1,1,1), 
		const d2d::Colorf& add = d2d::Colorf(0, 0, 0, 0),
		const d2d::Colorf& r_trans = d2d::Colorf(1, 0, 0, 0),
		const d2d::Colorf& g_trans = d2d::Colorf(0, 1, 0, 0),
		const d2d::Colorf& b_trans = d2d::Colorf(0, 0, 1, 0));
	static void getCurrSprites(const Symbol* symbol, int index, std::vector<d2d::ISprite*>& sprites);
	static Symbol::Frame* getCurrFrame(Symbol::Layer* layer, int index);
	static Symbol::Frame* getNextFrame(Symbol::Layer* layer, int index);

	static void getTweenSprites(const std::vector<d2d::ISprite*>& start, const std::vector<d2d::ISprite*>& end, 
		std::vector<d2d::ISprite*>& tween, float process);
	static bool isTweenMatched(const d2d::ISprite* s0, const d2d::ISprite* s1);
	static void getTweenSprite(d2d::ISprite* start, d2d::ISprite* end, d2d::ISprite* tween, float process);

}; // Tools

}

