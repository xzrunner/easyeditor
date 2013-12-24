#pragma once

#include "ISymbol.h"

namespace d2d
{
	class FontBlankSymbol : public ISymbol
	{
	public:
		FontBlankSymbol();
		virtual ~FontBlankSymbol();

		//
		// ICloneable interface
		//
		virtual FontBlankSymbol* clone() { return NULL; }

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

	protected:
		virtual void loadResources();

	public:
		std::string font;

		std::string color;

		float align;
		float size;

		float width, height;

	}; // FontBlankSymbol
}

