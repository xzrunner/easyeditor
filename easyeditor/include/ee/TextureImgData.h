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

	virtual std::string GetFilepath() const;

	virtual int GetWidth() const;
	virtual int GetHeight() const;
	virtual int GetFormat() const;

	virtual void LoadFromMemory(const uint8_t* pixels, int w, int h, int fmt);

private:
	std::string m_filepath;

	int m_width, m_height;
	int m_format;

	unsigned int m_texid;

}; // TextureImgData

}

#endif // _EASYEDITOR_TEXTURE_IMG_DATA_H_