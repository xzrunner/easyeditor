#include "RenderScreen.h"
#include "RenderCtxStack.h"

#include <shaderlab.h>

namespace s2
{

void RenderScreen::Scissor(float x, float y, float w, float h)
{
	const RenderCtx* ctx = RenderCtxStack::Instance()->Top();
	if (!ctx) {
		sl::ShaderMgr::Instance()->GetContext()->SetScissor(0, 0, 0, 0);
		return;
	}

	x *= ctx->mv_scale;
	y *= ctx->mv_scale;
	w *= ctx->mv_scale;
	h *= ctx->mv_scale;

	x += ctx->proj_width * 0.5f;
	y += ctx->proj_height * 0.5f;

	x += ctx->mv_offset.x * ctx->mv_scale;
	y += ctx->mv_offset.y * ctx->mv_scale;

	if (x < 0) {
		w += x;
		x = 0;
	} else if (x > ctx->proj_width) {
		w = h = 0;
	}
	if (y < 0) {
		h += y;
		y = 0;
	} else if (y > ctx->proj_height) {
		w = h = 0;
	}
	sl::ShaderMgr::Instance()->GetContext()->SetScissor(x, y, w, h);
}

}