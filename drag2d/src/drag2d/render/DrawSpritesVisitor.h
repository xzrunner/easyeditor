#pragma once

#include "interfaces.h"

namespace d2d
{
	class SpriteBatch;

	class DrawSpritesVisitor : public IVisitor
	{
	public:
		DrawSpritesVisitor(SpriteBatch& batch);
		virtual ~DrawSpritesVisitor();

		virtual void visit(Object* object, bool& bFetchNext);

	private:
		SpriteBatch& m_batch;

	}; // DrawSpritesVisitor
}

