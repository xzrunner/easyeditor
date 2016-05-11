#ifndef _EASYEDITOR_IMAGE_SYMBOL_H_
#define _EASYEDITOR_IMAGE_SYMBOL_H_

#include "Symbol.h"

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
	virtual void Draw(const s2::RenderParams& trans, const s2::Sprite* spr = NULL) const;
	virtual void ReloadTexture() const;
	virtual Rect GetSize(const Sprite* sprite = NULL) const;
	virtual void InvalidRect(const sm::mat4& mt) const;

	unsigned int GetTexID() const;

	Image* GetImage() const { return m_image; }

	void SetRegion(const Rect& region) { m_region = region; }

protected:
	virtual void LoadResources();

private:
	void InitRegion();

protected:
	Image* m_image;

	Rect m_region;

}; // ImageSymbol

}

#endif // _EASYEDITOR_IMAGE_SYMBOL_H_