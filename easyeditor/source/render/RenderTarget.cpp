//#include "RenderTarget.h"
//
//#include <shaderlab.h>
//#include <render/render.h>
//
//namespace ee
//{
//
//RenderTarget::RenderTarget()
//	: m_id(-1)
//	, m_width(0)
//	, m_height(0)
//{
//}
//
//RenderTarget::RenderTarget(int width, int height)
//	: m_width(width)
//	, m_height(height)
//{
//	m_id = Create(width, height);
//}
//
//RenderTarget::~RenderTarget()
//{	
//	Release(m_id);
//}
//
//void RenderTarget::Reize(int width, int height)
//{
//	m_width = width;
//	m_height = height;
//
//	Release(m_id);
//	m_id = Create(width, height);
//}
//
//void RenderTarget::DrawSprite(const Sprite* spr, bool clear = false, float dx = 0, float dy = 0)
//{
//
//}
//
//void RenderTarget::DrawSprite(const Sprite* spr, bool clear, int width, int height, float dx = 0, float dy = 0)
//{
//
//}
//
//void RenderTarget::DrawShape(const Shape* shape, bool clear, int width, int height)
//{
//
//}
//
//void RenderTarget::DrawSymbol(const Symbol* sym, bool whitebg = false, float scale = 1.0f)
//{
//
//}
//
//void RenderTarget::ReadPixels(unsigned char* pixels, int width, int height) const
//{
//
//}
//
//int RenderTarget::Create(int w, int h)
//{
//	render* r = sl_shader_get_render();
//	return render_target_create(r, w, h, TEXTURE_RGBA8);
//}
//
//void RenderTarget::Release(int id)
//{
//	if (id >= 0) {
//		render* r = sl_shader_get_render();
//		render_release(r, TARGET, id);
//	}
//}
//
//void RenderTarget::DrawFBO(const Symbol* sym, bool whitebg, float scale)
//{
//
//}
//
//void RenderTarget::DrawFBO(const Sprite* spr, bool clear, int width, int height, float dx, float dy)
//{
//
//}
//
//void RenderTarget::DrawFBO(const Shape* shape, bool clear, int width, int height)
//{
//
//}
//
//}