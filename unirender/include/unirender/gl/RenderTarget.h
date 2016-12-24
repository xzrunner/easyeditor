#ifndef _UNIRENDER_GL_RENDER_TARGET_H_
#define _UNIRENDER_GL_RENDER_TARGET_H_

namespace ur
{
namespace gl
{

class RenderTarget
{
public:
	RenderTarget();

	int  Create(int id);
	void Release(int id);

	void BindTarget(int id);
	void BindTexture(int tex_id);

	int  CheckStatus();

	// todo
	void SetCurr(int id) {
	//	render_set(RS->R, TARGET, id, 0);
		m_curr = id;	
	}
	int  GetCurr() const {
	//	return render_get(RS->R, TARGET, 0);
		return m_curr;
	}

private:
	int m_curr;

}; // RenderTarget

}
}

#endif // _UNIRENDER_GL_RENDER_TARGET_H_