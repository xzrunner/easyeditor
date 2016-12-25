#include "gl/State.h"

namespace ur
{
namespace gl
{

State::State(render* render, void (*state_change_cb)())
	: m_render(render)
	, m_state_change_cb(state_change_cb)
	, m_blend_src(BLEND_ONE)
	, m_blend_dst(BLEND_ONE_MINUS_SRC_ALPHA)
	, m_blend_func(BLEND_FUNC_ADD)
	, m_clear_mask(0)
{
	m_vp_x = m_vp_y = m_vp_w = m_vp_h = 0;

	render_set_blendfunc(render, m_blend_src, m_blend_dst);
	render_set_blendeq(render, m_blend_func);
}

void State::SetBlend(int m1, int m2)
{
	if (m1 == m_blend_src && m2 == m_blend_dst) {
		return;
	}

	m_state_change_cb();

	m_blend_src = static_cast<BLEND_FORMAT>(m1);
	m_blend_dst = static_cast<BLEND_FORMAT>(m2);
	render_set_blendfunc(m_render, m_blend_src, m_blend_dst);
}

void State::SetBlendEquation(int func)
{
	if (func == m_blend_func) {
		return;
	}

	m_state_change_cb();

	m_blend_func = static_cast<BLEND_FUNC>(func);
	render_set_blendeq(m_render, m_blend_func);
}

void State::SetDefaultBlend()
{
	SetBlend(BLEND_ONE, BLEND_ONE_MINUS_SRC_ALPHA);
	SetBlendEquation(BLEND_FUNC_ADD);
}

void State::SetClearFlag(int flag)
{
	m_clear_mask |= flag;
}

void State::Clear(unsigned long argb)
{
	render_clear(m_render, CLEAR_MASK(m_clear_mask), argb);
}

void State::EnableScissor(int enable)
{
	render_enablescissor(m_render, enable);
}

void State::SetScissor(int x, int y, int width, int height)
{
	render_setscissor(x, y, width, height);
}

void State::SetViewport(int x, int y, int w, int h)
{
	if (x == m_vp_x && y == m_vp_y &&
		w == m_vp_w && h == m_vp_h) {
		return;
	}

	m_vp_x = x;
	m_vp_y = y;
	m_vp_w = w;
	m_vp_h = h;

	render_setviewport(m_vp_x, m_vp_y, m_vp_w, m_vp_h);
}

void State::GetViewport(int& x, int& y, int& w, int& h)
{
	x = m_vp_x;
	y = m_vp_y;
	w = m_vp_w;
	h = m_vp_h;
}

void State::SetDepth(DEPTH_FORMAT d)
{
	render_setdepth(m_render, d);
}

}
}