#pragma once

#include "ISymbol.h"

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
		// ISerializable interface
		//
		virtual void loadFromTextFile(std::ifstream& fin);
		virtual void storeToTextFile(std::ofstream& fout) const;

		//
		// ISymbol interfaces
		//
		virtual void reloadTexture() const;
		virtual void draw(const ISprite* sprite = NULL) const;
		virtual float getWidth(const ISprite* sprite = NULL) const;
		virtual float getHeight(const ISprite* sprite = NULL) const;

		//
		// ListItem interface
		//
		virtual void refresh();

		bool loadFont(const std::string& filename);

	protected:
		virtual void loadResources();

	private:
		// from NeHe's FreeType
		void print(float x, float y, const char* text) const;

	public:
		std::string font;

		std::string color;

		float align;
		float size;

		float width, height;

		std::string filename;

	private:
		Font* m_font;

	}; // FontBlankSymbol
}

