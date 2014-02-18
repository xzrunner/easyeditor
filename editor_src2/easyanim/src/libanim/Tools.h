#pragma once

#include <drag2d.h>

#include "Symbol.h"

namespace anim
{

class Tools
{
public:
	static void drawAnimSymbol(const Symbol* symbol, int index);
	static void getCurrSprites(const Symbol* symbol, int index, std::vector<d2d::ISprite*>& sprites);
	static Symbol::Frame* getCurrFrame(Symbol::Layer* layer, int index);
	static Symbol::Frame* getNextFrame(Symbol::Layer* layer, int index);

	static void getTweenSprites(const std::vector<d2d::ISprite*>& start, const std::vector<d2d::ISprite*>& end, 
		std::vector<d2d::ISprite*>& tween, float process);
	static bool isTweenMatched(d2d::ISprite& s0, d2d::ISprite& s1);
	static void getTweenSprite(d2d::ISprite* start, d2d::ISprite* end, d2d::ISprite* tween, float process);

}; // Tools

}

