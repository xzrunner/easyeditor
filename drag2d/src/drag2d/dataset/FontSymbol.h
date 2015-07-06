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
		virtual FontSymbol* Clone() const;

		//
		// ISymbol interface
		//
		virtual void ReloadTexture() const;
		virtual void Draw(const Matrix& mt,
			const Colorf& mul = Colorf(1, 1, 1, 1), 
			const Colorf& add = Colorf(0, 0, 0, 0),
			const Colorf& r_trans = Colorf(1, 0, 0, 0),
			const Colorf& g_trans = Colorf(0, 1, 0, 0),
			const Colorf& b_trans = Colorf(0, 0, 1, 0),
			const ISprite* sprite = NULL) const;
		virtual Rect GetSize(const ISprite* sprite = NULL) const;

		// from NeHe's FreeType
		void print(float x, float y, const char* text) const;

	protected:
		virtual void LoadResources();

	private:
		int getStrWidth(const std::string& str) const;

	private:
		Font* m_font;

	}; // FontSymbol
}

