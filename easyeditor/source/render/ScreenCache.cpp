//#include "ScreenCache.h"
//#include "EE_SP.h"
//#include "Sprite.h"
//#include "Symbol.h"
//#include "Camera.h"
//
//#include <dtex_cs.h>
//#include <dtex_screen.h>
//#include <dtex_gl.h>
//#include <sprite2/RenderParams.h>
//
//#include <gl/glew.h>
//
//namespace ee
//{
//
//ScreenCache* ScreenCache::m_instance = NULL;
//
//static dtex_cs* CS;
//
//ScreenCache::ScreenCache()
//	: m_use_col_crading(false)
//{
//	CS = dtex_cs_create();
//}
//
//void ScreenCache::SetSize(int w, int h)
//{
//	m_width = w;
//	m_height = h;
//	dtex_cs_on_size(CS, w, h);
//}
//
//void ScreenCache::GetSize(int& w, int& h) const
//{
//	w = m_width;
//	h = m_height;
//}
//
//void ScreenCache::Reload()
//{
//	dtex_cs_reload(CS);
//}
//
//void ScreenCache::Bind()
//{
//	dtex_cs_bind(CS);
//}
//
//void ScreenCache::Unbind()
//{
//	dtex_cs_unbind(CS);
//}
//
//void ScreenCache::InvalidRect(float xmin, float ymin, float xmax, float ymax)
//{
//	dtex_cs_rect r;
//	r.xmin = xmin;
//	r.ymin = ymin;
//	r.xmax = xmax;
//	r.ymax = ymax;
//	dtex_cs_add_inv_rect(CS, &r);
//}
//
//static void _cs_draw(struct dtex_cs_rect* r, void* ud) 
//{
//	const Camera* cam = (Camera*)ud;
//
//	float cam_x = cam->GetPosition().x,
//		  cam_y = cam->GetPosition().y,
//		  cam_scale = cam->GetScale();
//
//	float scr_w, scr_h, scr_s;
//	dtex_get_screen(&scr_w, &scr_h, &scr_s);
//
//	int x = static_cast<int>((r->xmin - cam_x) / cam_scale + scr_w * 0.5f),
//		y = static_cast<int>((r->ymin - cam_y) / cam_scale + scr_h * 0.5f),
//		w = static_cast<int>((r->xmax - r->xmin) / cam_scale),
//		h = static_cast<int>((r->ymax - r->ymin) / cam_scale);
//	dtex_gl_scissor(x, y, w, h);
//	dtex_gl_clear_color(0, 1, 0, 1);
//
//	sm::rect rq;
//	rq.xmin = r->xmin;
//	rq.ymin = r->ymin;
//	rq.xmax = r->xmax;
//	rq.ymax = r->ymax;
//
//	std::vector<const Sprite*> sprs;
//	SpatialPartition::Instance()->Query(rq, sprs);
//
//	for (int i = 0, n = sprs.size(); i < n; ++i) {
//		const Sprite* spr = sprs[i];
//
//		s2::RenderParams params;
//		params.mt = spr->GetLocalMat();
//		spr->GetSymbol()->Draw(params);
//	}
//
//	mgr->GetContext()->Clear(0);
//}
//
//void ScreenCache::Draw(const Camera* cam)
//{
//	glEnable(GL_SCISSOR_TEST);
//	dtex_cs_traverse(CS, &_cs_draw, (Camera*)cam);
//	glDisable(GL_SCISSOR_TEST);
//}
//
//void ScreenCache::DrawToScreen(void (*before_draw)(void* ud), void* ud)
//{
//	dtex_cs_draw(CS, 1, 1, 1, 1, before_draw, ud);
//}
//
//int ScreenCache::GetTexID() const
//{
//	return dtex_cs_get_texture_id(CS);
//}
//
//ScreenCache* ScreenCache::Instance()
//{
//	if (!m_instance) {
//		m_instance = new ScreenCache();
//	}
//	return m_instance;
//}
//
//}