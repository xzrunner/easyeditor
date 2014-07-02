#pragma once

#include "ISymbol.h"
#include "render/ShapeStyle.h"

namespace d2d
{
	class Font;

	class FontBlankSymbol : public ISymbol
	{
	public:
		FontBlankSymbol();
		virtual ~FontBlankSymbol();

		//
		// ICloneable interface
		//
		virtual FontBlankSymbol* clone() const { return NULL; }

		//
		// ISymbol interfaces
		//
		virtual void reloadTexture() const;
		virtual void draw(const Screen& scr,
			const Matrix& mt,
			const Colorf& mul = Colorf(1, 1, 1, 1), 
			const Colorf& add = Colorf(0, 0, 0, 0),
			const ISprite* sprite = NULL) const;
		virtual Rect getSize(const ISprite* sprite = NULL) const;

		//
		// ListItem interface
		//
		virtual void refresh();

		bool loadFont(const std::string& filename);

	protected:
		virtual void loadResources();

	private:
		void DrawBackground(const Screen& scr, const ISprite* sprite) const;
		void DrawName(const ISprite* sprite) const;
		void DrawText(const ISprite* sprite) const;

		// from NeHe's FreeType
		void print(float x, float y, const char* text) const;

	public:
		std::string font;

		std::string color;

		float align_hori, align_vert;
		float size;

		float width, height;

		std::string filename;

	private:
		Font* m_font;

		ShapeStyle m_style;

	}; // FontBlankSymbol
}

