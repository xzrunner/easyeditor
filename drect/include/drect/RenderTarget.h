#ifndef _DRECT_RENDER_TARGET_H_
#define _DRECT_RENDER_TARGET_H_

namespace ur { class RenderTarget; class RenderContext; }

namespace drect
{

class RenderTarget
{
public:
	RenderTarget(ur::RenderContext* rc, int width, int height, int extend);
	~RenderTarget();

	void Bind();
	void Unbind();

	void SetCenter(int x, int y);

	int  GetTexID() const;

private:
	int m_cx, m_cy;

	int m_width, m_height;
	int m_extend;

	ur::RenderTarget* m_rt;

}; // RenderTarget

}

#endif // _DRECT_RENDER_TARGET_H_