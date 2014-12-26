#pragma once

#include <vector>

#include "common/Matrix.h"
#include "common/Color.h"

namespace d2d
{
	class ISprite;
	class ISymbol;
	class SpriteBatch;
	class Vector;
	class Matrix;

	class SpriteDraw
	{
	public:
		static void drawSprite(
			const ISprite* sprite, 
			const Matrix& mt = Matrix(),
			const Colorf& mul = Colorf(1,1,1,1),
			const Colorf& add = Colorf(0,0,0,0),
			const Colorf& r_trans = Colorf(1, 0, 0, 0),
			const Colorf& g_trans = Colorf(0, 1, 0, 0),
			const Colorf& b_trans = Colorf(0, 0, 1, 0));

		static void drawSprite(
			const ISymbol* symbol, 
			const d2d::Matrix& mt = Matrix(), 
			const Vector& pos = Vector(0, 0),
			float angle = 0.0f, 
			float xScale = 1.0f, 
			float yScale = 1.0f,
			float xShear = 0.0f, 
			float yShear = 0.0f, 
			const Colorf& mul = Colorf(1,1,1,1),
			const Colorf& add = Colorf(0,0,0,0),
			const Colorf& r_trans = Colorf(1, 0, 0, 0),
			const Colorf& g_trans = Colorf(0, 1, 0, 0),
			const Colorf& b_trans = Colorf(0, 0, 1, 0));

	}; // SpriteDraw
}

