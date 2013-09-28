#pragma once

#include "ISprite.h"

#include "FontSymbol.h"

#include "common/Color.h"

namespace d2d
{
	class TextSprite : public ISprite
	{
	public:
		TextSprite();
		TextSprite(const TextSprite& sprite);
		TextSprite(FontSymbol* symbol);

		//
		// IObject interface
		//
		virtual TextSprite* clone();

		//
		// ISprite interface
		//
		virtual const FontSymbol& getSymbol() const;
		virtual void setSymbol(ISymbol* symbol);
		virtual void loadBodyFromFile();
		virtual void setTransform(const Vector& position, float angle);

		void setText(const std::string& text);
		const std::string& getText() const {
			return m_text;
		}

		void setSize(int size);
		int getSize() const { return m_size; }

		void setColor(const d2d::Colori& color) { m_color = color; }
		const d2d::Colori& getColor() const { return m_color; }

		virtual void buildBounding();

	protected:
		FontSymbol* m_symbol;

	private:
		std::string m_text;

		int m_size;

		d2d::Colori m_color;

	}; // TextSprite
}


