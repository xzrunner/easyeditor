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
		virtual ~ImageSprite();

		//
		// ICloneable interface
		//
		virtual ImageSprite* Clone() const;

		//
		// ISprite interface
		//
		virtual const ImageSymbol& getSymbol() const;
		virtual void setSymbol(ISymbol* symbol);

		// todo: should auto resize through its symbol and shapes
		void buildBoundingFromTexCoords(float* texCoords);

	protected:
		ImageSymbol* m_symbol;

	}; // ImageSprite
}

