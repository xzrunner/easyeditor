#ifndef _GLUE_RENDER_CONTEXT_H_
#define _GLUE_RENDER_CONTEXT_H_

#include "utility/macro.h"

#include <render/render.h>

namespace glue
{

class Camera;

class RenderContext
{
public:
	void Init();

	void OnSize(int w, int h);
	void SetCamera(float x, float y, float sx, float sy);

	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }

	RID CreateTexture(const uint8_t* data, int width, int height, TEXTURE_FORMAT format);
	void ReleaseTexture(RID id);

	const Camera* GetCamera() const { return m_cam; }

private:
	Camera* m_cam;

	int m_width, m_height;

	SINGLETON_DECLARATION(RenderContext)
	
}; // RenderContext

}

#endif // _GLUE_RENDER_CONTEXT_H_