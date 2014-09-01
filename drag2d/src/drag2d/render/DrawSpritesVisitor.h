#pragma once

#include "interfaces.h"

namespace d2d
{
	class Screen;
	class SpriteBatch;

	class DrawSpritesVisitor : public IVisitor
	{
	public:
		DrawSpritesVisitor(const Screen& scr, SpriteBatch& batch);
		virtual ~DrawSpritesVisitor();

		virtual void visit(Object* object, bool& bFetchNext);

	private:
		const Screen& m_scr;

		SpriteBatch& m_batch;

	}; // DrawSpritesVisitor
}

