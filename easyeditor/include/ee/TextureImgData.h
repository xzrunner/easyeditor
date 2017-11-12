#ifndef _EASYEDITOR_TEXTURE_IMG_DATA_H_
#define _EASYEDITOR_TEXTURE_IMG_DATA_H_

#include "Texture.h"

namespace ee
{

class TextureImgData : public Texture
{
public:
	TextureImgData();
	~TextureImgData();
	
	virtual unsigned int GetTexID() const { return m_texid; }

	virtual int GetWidth() const override;
	virtual int GetHeight() const override;
	virtual int GetFormat() const override;

	virtual void LoadFromMemory(const uint8_t* pixels, int w, int h, int fmt) override;

private:
	int m_width, m_height;
	int m_format;

	unsigned int m_texid;

}; // TextureImgData

}

#endif // _EASYEDITOR_TEXTURE_IMG_DATA_H_