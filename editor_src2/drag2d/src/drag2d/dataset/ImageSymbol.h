#pragma once

#include "ISymbol.h"

namespace d2d
{
	class Image;

	class ImageSymbol : public ISymbol
	{
	public:
		ImageSymbol();
		ImageSymbol(Image* image, const std::string& filename);
		virtual ~ImageSymbol();

		//
		// IObject interface
		//	
		virtual ImageSymbol* clone() const;

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
		virtual Rect getSize(const ISprite* sprite = NULL) const;

		unsigned int getTextureID() const;

		Image* getImage() const { return m_image; }

		void setRegion(const Rect& region) { m_region = region; }

	protected:
		virtual void loadResources();

	protected:
		Image* m_image;

		Rect m_region;

	}; // ImageSymbol
}

