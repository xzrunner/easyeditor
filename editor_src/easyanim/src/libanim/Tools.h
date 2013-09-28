#pragma once

#include <drag2d.h>

namespace libanim
{
 	class Tools
 	{
 	public:
		static void drawAnimSymbol(const d2d::AnimSymbol* symbol, int index);
		static void getCurrSprites(const d2d::AnimSymbol* symbol, int index, std::vector<d2d::ISprite*>& sprites);
		static d2d::AnimSymbol::Frame* getCurrFrame(d2d::AnimSymbol::Layer* layer, int index);
		static d2d::AnimSymbol::Frame* getNextFrame(d2d::AnimSymbol::Layer* layer, int index);

 		static void getTweenSprites(const std::vector<d2d::ISprite*>& start, const std::vector<d2d::ISprite*>& end, 
 			std::vector<d2d::ISprite*>& tween, float process);
 		static bool isTweenMatched(d2d::ISprite& s0, d2d::ISprite& s1);
 		static void getTweenSprite(d2d::ISprite* start, d2d::ISprite* end, d2d::ISprite* tween, float process);
 
 	}; // Tools
}

