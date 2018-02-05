#pragma once

#include <SM_Vector.h>

#include <stdint.h>

namespace pt2
{

class Texture
{
public:
	Texture(uint16_t w, uint16_t h, uint32_t id, int format);
	virtual ~Texture() {}

	void Init(uint16_t w, uint16_t h, uint32_t id, int format);
	void InitOri(uint16_t ori_w, uint16_t ori_h);
	
	sm::vec2 GetSize() const {
		return sm::vec2(m_width, m_height);
	}
	sm::vec2 GetOriSize() const {
		return sm::vec2(m_ori_w, m_ori_h);
	}

	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }

	int GetFormat() const { return m_format; }

	uint32_t GetTexID() const { return m_tex_id; }

	bool IsLoadFinished() const { return m_load_finished; }
	void SetLoadFinished(bool finished) { m_load_finished = finished; }

private:
	uint16_t m_width, m_height;

	uint32_t m_tex_id;

	int      m_format;

	uint16_t m_ori_w, m_ori_h;
//		     m_ori_x, m_ori_y;

	bool m_load_finished;

}; // Texture

}