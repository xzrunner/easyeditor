#ifndef _UNIRENDER_GL_DRAW_H_
#define _UNIRENDER_GL_DRAW_H_

#include "VertexAttrib.h"

#include <ejoy2d/render.h>

#include <CU_Uncopyable.h>

#include <vector>

namespace ur
{
namespace gl
{

class Draw : private cu::Uncopyable
{
public:
	Draw(render* render);

	void DrawElements(DRAW_MODE mode, int fromidx, int ni);
	void DrawArrays(DRAW_MODE mode, int fromidx, int ni);

	int  CreateBuffer(RENDER_OBJ what, const void *data, int n, int stride);
	void ReleaseBuffer(RENDER_OBJ what, int id);
	void BindBuffer(RENDER_OBJ what, int id);
	void UpdateBuffer(int id, const void* data, int n);

	int  CreateVertexLayout(const std::vector<VertexAttrib>& va_list);
	void ReleaseVertexLayout(int id);
	void BindVertexLayout(int id);

private:
	render* m_render;

}; // Draw

}
}

#endif // _UNIRENDER_GL_DRAW_H_