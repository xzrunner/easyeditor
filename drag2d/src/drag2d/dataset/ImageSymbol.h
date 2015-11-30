#ifndef _DRAG2D_IMAGE_SYMBOL_H_
#define _DRAG2D_IMAGE_SYMBOL_H_

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
	// ICloneable interface
	//	
	virtual ImageSymbol* Clone() const;

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
	virtual void InvalidRect(const Matrix& mt) const;

	unsigned int GetTexID() const;

	Image* getImage() const { return m_image; }

	void setRegion(const Rect& region) { m_region = region; }

protected:
	virtual void LoadResources();

private:
	void InitRegion();

protected:
	Image* m_image;

	Rect m_region;

}; // ImageSymbol

}

#endif // _DRAG2D_IMAGE_SYMBOL_H_