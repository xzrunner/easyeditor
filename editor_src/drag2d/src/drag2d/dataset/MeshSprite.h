#pragma once

#include "ISprite.h"

#include "MeshSymbol.h"

namespace d2d
{
	class MeshSprite;

	class MeshSprite : public ISprite
	{
	public:
		MeshSprite();
		MeshSprite(const MeshSprite& sprite);
		MeshSprite(MeshSymbol* symbol);
		virtual ~MeshSprite();

		//
		// IObject interface
		//
		virtual MeshSprite* clone();

		//
		// Sprite interface
		//
		virtual const MeshSymbol& getSymbol() const;
		virtual void setSymbol(ISymbol* symbol);
		virtual void loadBodyFromFile();

		virtual void buildBounding();

	private:
		MeshSymbol* m_symbol;

	}; // MeshSprite
}

