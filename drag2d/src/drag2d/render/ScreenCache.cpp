#include "ScreenCache.h"
#include "SpatialIndex.h"

#include "common/Rect.h"
#include "common/Matrix.h"
#include "dataset/ISprite.h"
#include "dataset/ISymbol.h"
#include "view/Camera.h"
#include "render/ShaderContext.h"

#include <dtex_cs.h>
#include <dtex_screen.h>
#include <dtex_gl.h>

#include <gl/glew.h>

namespace d2d
{

ScreenCache* ScreenCache::m_instance = NULL;

static dtex_cs* CS;

ScreenCache::ScreenCache()
{
	CS = dtex_cs_create();
}

void ScreenCache::OnSize(int w, int h)
{
	dtex_cs_on_size(CS, w, h);
}

void ScreenCache::Reload()
{
	dtex_cs_reload(CS);
}

void ScreenCache::Bind()
{
	dtex_cs_bind(CS);
}

void ScreenCache::Unbind()
{
	dtex_cs_unbind(CS);
}

void ScreenCache::InvalidRect(float xmin, float ymin, float xmax, float ymax)
{
	dtex_cs_rect r;
	r.xmin = xmin;
	r.ymin = ymin;
	r.xmax = xmax;
	r.ymax = ymax;
	dtex_cs_add_inv_rect(CS, &r);
}

static void _cs_draw(struct dtex_cs_rect* r, void* ud) 
{
	const Camera* cam = (Camera*)ud;

	float cam_x = cam->GetPosition().x,
		  cam_y = cam->GetPosition().y,
		  cam_scale = cam->GetScale();

	float scr_w, scr_h, scr_s;
	dtex_get_screen(&scr_w, &scr_h, &scr_s);

	float x = (r->xmin - cam_x) / cam_scale + scr_w * 0.5f,
		y = (r->ymin - cam_y) / cam_scale + scr_h * 0.5f,
		w = (r->xmax - r->xmin) / cam_scale,
		h = (r->ymax - r->ymin) / cam_scale;
	dtex_gl_scissor(x, y, w, h);
	dtex_gl_clear_color(0, 1, 0, 1);

	Rect rq;
	rq.xMin = r->xmin;
	rq.yMin = r->ymin;
	rq.xMax = r->xmax;
	rq.yMax = r->ymax;

	std::vector<const ISprite*> sprites;
	SpatialIndex::Instance()->Query(rq, sprites);

	for (int i = 0, n = sprites.size(); i < n; ++i) {
		const ISprite* spr = sprites[i];

		Matrix t;
		spr->GetTransMatrix(t);
		spr->GetSymbol().Draw(t);
	}

	ShaderContext::Flush();
}

void ScreenCache::Draw(const Camera* cam)
{
	glEnable(GL_SCISSOR_TEST);
	dtex_cs_traverse(CS, &_cs_draw, (Camera*)cam);
	glDisable(GL_SCISSOR_TEST);
}

void ScreenCache::DrawToScreen(void (*before_draw)(void* ud), void* ud)
{
	dtex_cs_draw_to_screen(CS, before_draw, ud);
}

ScreenCache* ScreenCache::Instance()
{
	if (!m_instance) {
		m_instance = new ScreenCache();
	}
	return m_instance;
}

}