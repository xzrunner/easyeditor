#ifndef _EASYEDITOR_IMAGE_SYMBOL_H_
#define _EASYEDITOR_IMAGE_SYMBOL_H_

#include "Symbol.h"

namespace s2 { class ImageSymbol; }

namespace ee
{

class Image;

class ImageSymbol : public Symbol
{
public:
	ImageSymbol();
	ImageSymbol(Image* image, const std::string& filename);
	virtual ~ImageSymbol();

	//
	// Cloneable interface
	//	
	virtual ImageSymbol* Clone() const;

	//
	// Symbol interface
	//
	virtual void Draw(const s2::RenderParams& params, const Sprite* spr = NULL) const;
	virtual void ReloadTexture() const;
	virtual sm::rect GetSize(const Sprite* sprite = NULL) const;
	virtual void InvalidRect(const sm::mat4& mt) const;

	unsigned int GetTexID() const;

	Image* GetImage() const { return m_image; }

protected:
	virtual void LoadResources();

private:
	void InitCoreTex();

protected:
	Image* m_image;

private:
	s2::ImageSymbol* m_core;

}; // ImageSymbol

}

#endif // _EASYEDITOR_IMAGE_SYMBOL_H_