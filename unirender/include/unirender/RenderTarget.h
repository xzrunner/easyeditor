#ifndef _UNIRENDER_RENDER_TARGET_H_
#define _UNIRENDER_RENDER_TARGET_H_

#include <CU_Uncopyable.h>

namespace ur
{

class RenderContext;
class Texture;

class RenderTarget : private cu::Uncopyable
{
public:
	RenderTarget(RenderContext* rc, int width, int height);
	~RenderTarget();

	void Bind();
	void Unbind();

	const Texture* GetTexture() const { return m_texture; }
	
private:
	RenderContext* m_rc;

	Texture* m_texture;

	int m_id;

}; // RenderTarget

}

#endif // _UNIRENDER_RENDER_TARGET_H_