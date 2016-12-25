#ifndef _UNIRENDER_GL_RENDER_CONTEXT_H_
#define _UNIRENDER_GL_RENDER_CONTEXT_H_

#include "IRenderContext.h"

struct render;

namespace ur
{
namespace gl
{

class Texture;
class RenderTarget;
class Shader;
class State;
class Draw;

class RenderContext : public IRenderContext
{
public:
	RenderContext(const IRenderContext::Callback& cb);
	virtual ~RenderContext();
	
	virtual int RenderVersion() const;

	/************************************************************************/
	/* Texture                                                              */
	/************************************************************************/

	virtual int  CreateTexture(const void* pixels, int width, int height, int format);
	virtual void ReleaseTexture(int id);

	virtual void UpdateTexture(const void* pixels, int w, int h, unsigned int id);
	virtual void UpdateSubTexture(const void* pixels, int x, int y, int w, int h, unsigned int id);

	virtual void BindTexture(int id, int channel);

	virtual void ClearTextureCache();

	virtual int  GetCurrTexture() const;

	/************************************************************************/
	/* RenderTarget                                                         */
	/************************************************************************/

	virtual int  CreateRT(int id);
	virtual void ReleaseRT(int id);

	virtual void BindRT(int id);
	virtual void BindRTTexture(int tex_id);

	virtual int  CheckRTStatus();

	virtual void SetCurrRT(int id);
	virtual int  GetCurrRT() const;

	/************************************************************************/
	/* Shader                                                               */
	/************************************************************************/

	virtual int  CreateShader(const char* vs, const char* fs);
	virtual void ReleaseShader(int id);

	virtual void BindShader(int id);

	virtual int  GetShaderUniform(const char* name);
	virtual void SetShaderUniform(int loc, UNIFORM_FORMAT format, const float* v);

	/************************************************************************/
	/* State                                                                */
	/************************************************************************/

	virtual void SetBlend(int m1, int m2);
	virtual void SetBlendEquation(int func);
	virtual void SetDefaultBlend();

	virtual void SetClearFlag(int flag);
	virtual void Clear(unsigned long argb);

	virtual void EnableScissor(int enable);
	virtual void SetScissor(int x, int y, int width, int height);

	virtual void SetViewport(int x, int y, int w, int h);
	virtual void GetViewport(int& x, int& y, int& w, int& h);

	virtual void SetDepth(DEPTH_FORMAT d);

	/************************************************************************/
	/* Draw                                                                 */
	/************************************************************************/

	virtual void DrawElements(DRAW_MODE mode, int fromidx, int ni);
	virtual void DrawArrays(DRAW_MODE mode, int fromidx, int ni);

	virtual int  CreateBuffer(RENDER_OBJ what, const void *data, int n, int stride);
	virtual void ReleaseBuffer(RENDER_OBJ what, int id);
	virtual void BindBuffer(RENDER_OBJ what, int id);
	virtual void UpdateBuffer(int id, const void* data, int n);

	virtual int  CreateVertexLayout(const std::vector<VertexAttrib>& va_list);
	virtual void ReleaseVertexLayout(int id);
	virtual void BindVertexLayout(int id);

private:
	render* m_render;
	
	Texture*      m_texture;
	RenderTarget* m_rt;
	Shader*       m_shader;
	State*        m_state;
	Draw*         m_draw;

}; // RenderContext

}
}

#endif // _UNIRENDER_GL_RENDER_CONTEXT_H_