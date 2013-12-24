#pragma once

#include "ISymbol.h"
#include "Image.h"

namespace d2d
{
	class Image;

	class ImageSymbol : public ISymbol
	{
	public:
		ImageSymbol();
		virtual ~ImageSymbol();

		//
		// IObject interface
		//	
		virtual ImageSymbol* clone();

		//
		// ISerializable interface
		//
		virtual void loadFromTextFile(std::ifstream& fin);
		virtual void storeToTextFile(std::ofstream& fout) const;

		//
		// ISymbol interface
		//
		virtual void reloadTexture() const;
		virtual void draw(const ISprite* sprite = NULL) const;
		virtual float getWidth(const ISprite* sprite = NULL) const;
		virtual float getHeight(const ISprite* sprite = NULL) const;

		unsigned int getTextureID() const;

		Image* getImage() { return m_image; }

	protected:
		virtual void loadResources();

	protected:
		Image* m_image;

	}; // ImageSymbol
}

