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
		// ICloneable interface
		//
		virtual MeshSprite* clone() const;

		//
		// Sprite interface
		//
		virtual const MeshSymbol& getSymbol() const;
		virtual void setSymbol(ISymbol* symbol);
		virtual void loadBodyFromFile();

	private:
		MeshSymbol* m_symbol;

	}; // MeshSprite
}

