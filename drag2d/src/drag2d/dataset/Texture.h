#ifndef _DRAG2D_TEXTURE_H_
#define _DRAG2D_TEXTURE_H_

typedef unsigned char uint8_t;

#include <string>

namespace d2d
{

class ImageData;

class Texture
{
public:
	Texture();
	~Texture();
	
	unsigned int GetTexID() const { return m_texid; }

	const std::string& GetFilepath() const;

	const uint8_t* GetPixelData() const;
	int GetWidth() const;
	int GetHeight() const;
	int GetChannels() const;

	void LoadFromFile(const std::string& filepath);
	void LoadFromMemory(ImageData* img_data);
	void Reload();

private:
	unsigned int m_texid;

	ImageData* m_img_data;

}; // Texture

}

#endif // _DRAG2D_TEXTURE_H_