#ifndef _SPRITE2_IMAGE_SYMBOL_H_
#define _SPRITE2_IMAGE_SYMBOL_H_

#include "S2_Symbol.h"

#include <stdint.h>

namespace s2
{

class Texture;

class ImageSymbol : public Symbol
{
public:
	struct Quad
	{
		uint16_t xmin, ymin;
		uint16_t xmax, ymax;
	}; // Quad

public:
	ImageSymbol(void* ud,
				void  (*query_texcoords)(void* ud, float* texcoords, int* texid),
				void  (*proj2screen)(float px, float py, int w, int h, float* sx, float* sy), 
				bool  (*is_ortho_cam)(),
				void  (*get_screen_size)(int* w, int* h),
				float (*get_p3d_cam_angle)(),
				int   (*get_screen_cache_texid)());

	virtual void Draw(const RenderParams& params, const Sprite* spr) const;

	void InitTex(Texture* tex, const Quad& quad, const sm::vec2& offset);

private:
	void DrawBlend(const RenderParams& params, sm::vec2* vertices, float* texcoords, int texid) const;
	void DrawOrtho(const RenderParams& params, sm::vec2* vertices, float* texcoords, int texid) const;
	void DrawPseudo3D(const RenderParams& params, sm::vec2* vertices, float* texcoords, int texid) const;

	void DrawGaussianBlur(const RenderParams& params, sm::vec2* vertices, float* texcoords, int texid) const;

protected:
	Texture*	m_tex;

	Quad		m_quad;
	sm::vec2	m_offset;

	void  (*m_query_texcoords)(void* ud, float* texcoords, int* texid);
	void  (*m_proj2screen)(float px, float py, int w, int h, float* sx, float* sy);
	bool  (*m_is_ortho_cam)();
	void  (*m_get_screen_size)(int* w, int* h);
	float (*m_get_p3d_cam_angle)();
	int   (*m_get_screen_cache_texid)();

}; // ImageSymbol

}

#endif // _SPRITE2_IMAGE_SYMBOL_H_