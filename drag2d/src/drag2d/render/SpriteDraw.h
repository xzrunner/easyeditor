#pragma once

#include <vector>

#include "Shader.h"

class wxMemoryDC;

namespace d2d
{
	class ISprite;
	class ISymbol;
	class SpriteBatch;
	class Vector;
	class Screen;

	class SpriteDraw
	{
	public:
		static void drawSprite(const ISprite* sprite, 
			const Colorf& mul = Colorf(1,1,1,1),
			const Colorf& add = Colorf(0,0,0,0));

		static void drawSprite(const ISymbol* symbol, const Vector& pos,
			float angle = 0.0f, float xScale = 1.0f, float yScale = 1.0f,
			float xShear = 0.0f, float yShear = 0.0f, const Colorf& mul = Colorf(1,1,1,1),
			const Colorf& add = Colorf(0,0,0,0));

		static void drawSprite(const Screen& scr,
			const ISprite* sprite, 
			const Colorf& mul = Colorf(1,1,1,1),
			const Colorf& add = Colorf(0,0,0,0));

		static void drawSprite(const Screen& scr, const ISymbol* symbol, const Vector& pos,
			float angle = 0.0f, float xScale = 1.0f, float yScale = 1.0f,
			float xShear = 0.0f, float yShear = 0.0f, const Colorf& mul = Colorf(1,1,1,1),
			const Colorf& add = Colorf(0,0,0,0));

		static void drawSprite(const ISprite* sprite, wxMemoryDC& memDC);
		static void drawSprite(const ISprite* sprite, const Vector& offset, wxMemoryDC& memDC,
			float bgwidth = 800, float bgheight = 480, float scale = 0.15f);

// 		static void drawSprites(const std::vector<ISprite*>& sprites, 
// 			SpriteBatch& batch);

		static void begin(const ISprite* sprite);
		static void end(const ISprite* sprite);

	}; // SpriteDraw
}

