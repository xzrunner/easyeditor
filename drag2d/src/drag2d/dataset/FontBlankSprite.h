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
		virtual FontBlankSprite* Clone() const;

		//
		// ISprite interface
		//
		virtual const FontBlankSymbol& getSymbol() const;
		virtual void setSymbol(ISymbol* symbol);

	protected:
		FontBlankSymbol* m_symbol;

	}; // FontBlankSprite
}

