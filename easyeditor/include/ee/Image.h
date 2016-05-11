#ifndef _EASYEDITOR_IMAGE_H_
#define _EASYEDITOR_IMAGE_H_

#include "ResourcesMgr.h"
#include "Object.h"

#include <SM_Matrix.h>

#include <stdint.h>

namespace ee
{

class Sprite;
class Texture;
class ImageData;
class FBO;
class RenderParams;

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

	void Draw(const RenderParams& trans, const Sprite* spr = NULL, 
		const Sprite* root = NULL) const;
	void InvalidRect(const sm::mat4& mt) const;

	const sm::vec2& GetOffset() const { return m_offset; }

	const ImageData* GetImageData() const;

private:
	void LoadWithClip(const std::string& filepath);

	bool CanUseDTex() const;

private:
	Texture* m_tex;

	float m_ori_w, m_ori_h;

	sm::vec2 m_offset;

}; // Image

typedef ResourcesMgr<Image> ImageMgr;

}

#endif // _EASYEDITOR_IMAGE_H_