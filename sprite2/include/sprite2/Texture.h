#ifndef _SPRITE2_TEXTURE_H_
#define _SPRITE2_TEXTURE_H_

#include <stdint.h>

namespace s2
{

class Texture
{
public:
	Texture(uint16_t w, uint16_t h, uint32_t id) {
		Init(w, h, id);
	}

	void Init(uint16_t w, uint16_t h, uint32_t id) {
		m_width = w;
		m_height = h;
		m_tex_id = id;
	}
	
	void GetSize(uint16_t& w, uint16_t& h) const {
		w = m_width;
		h = m_height;
	}

	uint32_t GetTexID() const { return m_tex_id; }

private:
	uint16_t m_width, m_height;

	uint32_t m_tex_id;

}; // Texture

}

#endif // _SPRITE2_TEXTURE_H_