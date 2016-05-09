#ifndef _EASYEDITOR_RENDER_CONTEXT_H_
#define _EASYEDITOR_RENDER_CONTEXT_H_

#include <SM_Vector.h>

namespace ee
{

class RenderContext
{
public:
	RenderContext();

	void SetModelView(const sm::vec2& offset, float scale);
	void SetProjection(int width, int height);

	bool GetModelView(sm::vec2& offset, float& scale) const;
	bool GetProjection(int& width, int& height) const;

	void OnBind();

private:
	sm::vec2 m_mod_offset;
	float m_mod_scale;

	int m_proj_width, m_proj_height;

}; // RenderContext

}

#endif // _EASYEDITOR_RENDER_CONTEXT_H_