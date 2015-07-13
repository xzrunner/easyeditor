#ifndef _DRAG2D_TEXTURE_H_
#define _DRAG2D_TEXTURE_H_

typedef unsigned char uint8_t;

#include <string>

namespace d2d
{

class Texture
{
public:
	Texture();
//	Texture(const uint8_t* pixel, int width, int height);
	~Texture();
	
	const std::string& GetFilepath() const { return m_filepath; }
	unsigned int GetTexID() const { return m_texid; }
	int GetChannels() const { return m_channels; }
	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }
	const uint8_t* GetPixelData() const { return m_pixels; }

	void LoadFromFile(const std::string& filepath);
	void LoadFromMemory(const uint8_t* pixels, int w, int h, int c, int f);
	void Reload();

	// todo for key
	void SetFilepath(const std::string& filepath) {
		m_filepath = filepath;
	}

private:
	std::string m_filepath;

	unsigned int m_texid;

	const uint8_t* m_pixels;
	int m_width, m_height;
	int m_channels;
	int m_format;

}; // Texture

}

#endif // _DRAG2D_TEXTURE_H_