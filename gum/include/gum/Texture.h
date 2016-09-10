#ifndef _GUM_TEXTURE_H_
#define _GUM_TEXTURE_H_

#include <string>

#include <render/render.h>

namespace gum
{

class Texture
{
public:
	Texture();
	~Texture();

	void Load(const std::string& filepath);
	void Load(const uint8_t* data, int width, int height, TEXTURE_FORMAT format);

	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }

	RID GetID() const { return m_id; }

private:
	void Load();

private:
	int m_width, m_height;
	TEXTURE_FORMAT m_format;

	RID m_id;

}; // Texture

}

#endif // _GUM_TEXTURE_H_
