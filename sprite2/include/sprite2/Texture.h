#ifndef _SPRITE2_TEXTURE_H_
#define _SPRITE2_TEXTURE_H_

#include <SM_Vector.h>
#include <CU_RefCountObj.h>

#include <stdint.h>

namespace s2
{

class Texture : public cu::RefCountObj
{
public:
	Texture(uint16_t w, uint16_t h, uint32_t id);

	void Init(uint16_t w, uint16_t h, uint32_t id);
	void InitOri(uint16_t ori_w, uint16_t ori_h);
	
	sm::vec2 GetSize() const {
		return sm::vec2(m_width, m_height);
	}
	sm::vec2 GetOriSize() const {
		return sm::vec2(m_ori_w, m_ori_h);
	}

	uint32_t GetTexID() const { return m_tex_id; }

private:
	uint16_t m_width, m_height;

	uint32_t m_tex_id;

	uint16_t m_ori_w, m_ori_h;
//		     m_ori_x, m_ori_y;

}; // Texture

}

#endif // _SPRITE2_TEXTURE_H_