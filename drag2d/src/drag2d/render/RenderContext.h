#ifndef _D2D_RENDER_CONTEXT_H_
#define _D2D_RENDER_CONTEXT_H_

#include "common/Vector.h"

namespace d2d
{

class RenderContext
{
public:
	RenderContext();

	void SetModelView(const Vector& offset, float scale);
	void SetProjection(int width, int height);

	void GetModelView(Vector& offset, float& scale) { 
		offset = m_mod_offset; 
		scale = m_mod_scale;
	}
	void GetProjection(int& width, int& height) { 
		width = m_proj_width; 
		height = m_proj_height;
	}

	static void SetCurrContext(RenderContext* context) { m_current = context; }
	static RenderContext* GetCurrContext() { return m_current; }

private:
	Vector m_mod_offset;
	float m_mod_scale;

	int m_proj_width, m_proj_height;

private:
	static RenderContext* m_current;

}; // RenderContext

}

#endif // _D2D_RENDER_CONTEXT_H_