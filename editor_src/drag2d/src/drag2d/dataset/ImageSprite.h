#pragma once

#include "ISprite.h"

#include "ImageSymbol.h"

namespace d2d
{
	class ImageSymbol;

	class ImageSprite : public ISprite
	{
	public:
		ImageSprite();
		ImageSprite(const ImageSprite& sprite);
		ImageSprite(ImageSymbol* symbol);

		//
		// IObject interface
		//
		virtual ImageSprite* clone();

		//
		// ISprite interface
		//
		virtual const ImageSymbol& getSymbol() const;
		virtual void setSymbol(ISymbol* symbol);
		virtual void loadBodyFromFile();

		// todo: should auto resize through its symbol and shapes
		void buildBounding(float* texCoords);

		virtual void buildBounding();

	protected:
		ImageSymbol* m_symbol;

	}; // ImageSprite
}

