#ifndef _UNIRENDER_GL_STATE_H_
#define _UNIRENDER_GL_STATE_H_

#include <ejoy2d/render.h>

#include <CU_Uncopyable.h>

namespace ur
{
namespace gl
{

class State : private cu::Uncopyable
{
public:
	State(render* render, void (*state_change_cb)());

	void SetBlend(int m1, int m2);
	void SetBlendEquation(int func);
	void SetDefaultBlend();

	void SetClearFlag(int flag);
	void Clear(unsigned long argb);

	void EnableScissor(int enable);
	void SetScissor(int x, int y, int width, int height);

	void SetViewport(int x, int y, int w, int h);
	void GetViewport(int& x, int& y, int& w, int& h);

private:
	render* m_render;
	void (*m_state_change_cb)();

	BLEND_FORMAT m_blend_src, m_blend_dst;
	BLEND_FUNC   m_blend_func;

	int m_clear_mask;

	int m_vp_x, m_vp_y, m_vp_w, m_vp_h;

}; // State

}
}

#endif // _UNIRENDER_GL_STATE_H_