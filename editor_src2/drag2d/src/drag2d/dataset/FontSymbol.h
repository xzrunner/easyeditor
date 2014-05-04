#pragma once

#include "ISymbol.h"

namespace d2d
{
	class Font;

	class FontSymbol : public ISymbol
	{
	public:
		FontSymbol();
		virtual ~FontSymbol();

		//
		// ICloneable interface
		//	
		virtual FontSymbol* clone() const;

		//
		// ISymbol interface
		//
		virtual void reloadTexture() const;
		virtual void draw(const Colorf& mul = Colorf(1, 1, 1, 1), 
			const Colorf& add = Colorf(0, 0, 0, 0),
			const ISprite* sprite = NULL) const;
		virtual Rect getSize(const ISprite* sprite = NULL) const;

		// from NeHe's FreeType
		void print(float x, float y, const char* text) const;

	protected:
		virtual void loadResources();

	private:
		Font* m_font;

	}; // FontSymbol
}

