#ifndef _DRAG2D_IMAGE_H_
#define _DRAG2D_IMAGE_H_

#include "Texture.h"

#include "common/ResourcesMgr.h"
#include "common/Vector.h"
#include "common/Object.h"

namespace d2d
{

class Matrix;
class ISprite;

class Image : public Object
{
public:
	Image() {}
	Image(ImageData* img_data);
	~Image();
	
	bool LoadFromFile(const std::string& filepath);
	void ReloadTexture();

	const std::string& GetFilepath() const { return m_tex.GetFilepath(); }
	unsigned int GetTexID() const { return m_tex.GetTexID(); }
	int GetChannels() const { return m_tex.GetChannels(); }
	int GetOriginWidth() const { return m_ori_w; }
	int GetOriginHeight() const { return m_ori_h; }
	int GetClippedWidth() const { return m_tex.GetWidth(); }
	int GetClippedHeight() const { return m_tex.GetHeight(); }
	const uint8_t* GetPixelData() const { return m_tex.GetPixelData(); }

	void Draw(const Matrix& mt, const ISprite* spr = NULL) const;

	const Vector& GetOffset() const { return m_offset; }

	const ImageData* GetImageData() const { return m_tex.GetImageData(); }

private:
	void LoadWithClip(const std::string& filepath);

private:
	Texture m_tex;

	float m_ori_w, m_ori_h;

	Vector m_offset;

}; // Image

typedef ResourcesMgr<Image> ImageMgr;

}

#endif // _DRAG2D_IMAGE_H_