#include "gl/Draw.h"
#include "gl/typedef.h"

namespace ur
{
namespace gl
{

Draw::Draw(render* render)
	: m_render(render)
{
}

void Draw::DrawElements(DRAW_MODE mode, int fromidx, int ni)
{
	render_draw_elements(m_render, (DRAW_MODE)mode, fromidx, ni);
}

void Draw::DrawArrays(DRAW_MODE mode, int fromidx, int ni)
{
	render_draw_arrays(m_render, mode, fromidx, ni);
}

int  Draw::CreateBuffer(RENDER_OBJ what, const void* data, int n, int stride)
{
	return render_buffer_create(m_render, what, NULL, n, stride);
}

void Draw::ReleaseBuffer(RENDER_OBJ what, int id)
{
	render_release(m_render, what, id);
}

void Draw::BindBuffer(RENDER_OBJ what, int id)
{
	render_set(m_render, what, id, 0);
}

void Draw::UpdateBuffer(int id, const void* data, int n)
{
	render_buffer_update(m_render, id, data, n);
}

int  Draw::CreateVertexLayout(const std::vector<VertexAttrib>& va_list)
{
	struct vertex_attrib va[MAX_LAYOUT];
	int offset = 0;
	for (int i = 0, n = va_list.size(); i < n; ++i) 
	{
		const VertexAttrib& src = va_list[i];
		vertex_attrib& dst = va[i];
		dst.name = src.name.c_str();
		dst.vbslot = 0;	// todo
		dst.n = src.n;
		dst.size = src.size;
		dst.offset = offset;
		offset += src.tot_size;
	}

	return render_register_vertexlayout(m_render, va_list.size(), va);
}

void Draw::ReleaseVertexLayout(int id)
{
	render_release(m_render, VERTEXLAYOUT, id);
}

void Draw::BindVertexLayout(int id)
{
	render_set(m_render, VERTEXLAYOUT, id, 0);
}

}
}