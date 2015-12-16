#ifndef _DRAG2D_IMAGE_H_
#define _DRAG2D_IMAGE_H_

#include "common/ResourcesMgr.h"
#include "common/Vector.h"
#include "common/Object.h"

#include <stdint.h>

namespace d2d
{

class Matrix;
class ISprite;
class ITexture;
class ImageData;
class FBO;

class Image : public Object
{
public:
	Image();
	Image(ImageData* img_data);
	Image(const FBO* fbo);
	~Image();
	
	bool LoadFromFile(const std::string& filepath);
	void ReloadTexture();

	std::string GetFilepath() const;
	unsigned int GetTexID() const;
	int GetChannels() const;
	int GetOriginWidth() const;
	int GetOriginHeight() const;
	int GetClippedWidth() const;
	int GetClippedHeight() const;
	const uint8_t* GetPixelData() const;

	void Draw(const Matrix& mt, const ISprite* spr = NULL, const ISprite* root = NULL) const;
	void InvalidRect(const Matrix& mt) const;

	const Vector& GetOffset() const { return m_offset; }

	const ImageData* GetImageData() const;

private:
	void LoadWithClip(const std::string& filepath);

	bool CanUseDTex() const;

private:
	ITexture* m_tex;

	float m_ori_w, m_ori_h;

	Vector m_offset;

}; // Image

typedef ResourcesMgr<Image> ImageMgr;

}

#endif // _DRAG2D_IMAGE_H_