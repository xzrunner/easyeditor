#ifndef _DRAG2D_IMAGE_H_
#define _DRAG2D_IMAGE_H_

#include "Texture.h"

#include "common/ResourcesMgr.h"
#include "common/Rect.h"
#include "common/Object.h"

namespace d2d
{

class Matrix;
class ISprite;

class Image : public Object
{
public:
	Image() {}
	Image(const uint8_t* pixel, int width, int height);
	~Image();
	
	bool LoadFromFile(const std::string& filepath);
	void Reload();

	const std::string& GetFilepath() const { return m_tex.GetFilepath(); }
	unsigned int GetTexID() const { return m_tex.GetTexID(); }
	int GetChannels() const { return m_tex.GetChannels(); }
	int GetOriginWidth() const { return m_tex.GetWidth(); }
	int GetOriginHeight() const { return m_tex.GetHeight(); }
	int GetClippedWidth() const { return m_clipped_region.xLength(); }
	int GetClippedHeight() const { return m_clipped_region.yLength(); }
	const uint8_t* GetPixelData() const { return m_tex.GetPixelData(); }

	void Draw(const Matrix& mt, const Rect& r, const ISprite* spr = NULL) const;

	Rect GetClippedRegion() const { return m_clipped_region; }

private:
	void ResetClippedRegion();

private:
	Texture m_tex;

	Rect m_clipped_region;

}; // Image

typedef ResourcesMgr<Image> ImageMgr;

}

#endif // _DRAG2D_IMAGE_H_