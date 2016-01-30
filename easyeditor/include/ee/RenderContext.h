#ifndef _EASYEDITOR_RENDER_CONTEXT_H_
#define _EASYEDITOR_RENDER_CONTEXT_H_

#include "common/Vector.h"

namespace ee
{

class RenderContext
{
public:
	RenderContext();

	void SetModelView(const Vector& offset, float scale);
	void SetProjection(int width, int height);

	bool GetModelView(Vector& offset, float& scale) const;
	bool GetProjection(int& width, int& height) const;

private:
	Vector m_mod_offset;
	float m_mod_scale;

	int m_proj_width, m_proj_height;

}; // RenderContext

}

#endif // _EASYEDITOR_RENDER_CONTEXT_H_