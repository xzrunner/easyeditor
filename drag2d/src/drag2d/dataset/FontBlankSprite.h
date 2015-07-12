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
		virtual bool Update(int version) { return false; }
		virtual const FontBlankSymbol& GetSymbol() const;
		virtual void SetSymbol(ISymbol* symbol);

	protected:
		FontBlankSymbol* m_symbol;

	}; // FontBlankSprite
}

