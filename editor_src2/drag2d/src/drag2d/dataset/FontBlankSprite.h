#pragma once

#include "ISprite.h"

#include "FontBlankSymbol.h"

namespace d2d
{
	class FontBlankSprite : public ISprite
	{
	public:
		FontBlankSprite();
		FontBlankSprite(const FontBlankSprite& sprite);
		FontBlankSprite(FontBlankSymbol* symbol);
		virtual ~FontBlankSprite();

		//
		// IObject interface
		//
		virtual FontBlankSprite* clone() const;

		//
		// ISprite interface
		//
		virtual const FontBlankSymbol& getSymbol() const;
		virtual void setSymbol(ISymbol* symbol);
		virtual void loadBodyFromFile();

		virtual void buildBounding();

	protected:
		FontBlankSymbol* m_symbol;

	}; // FontBlankSprite
}

