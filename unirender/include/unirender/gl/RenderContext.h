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

public:
	static const int MAX_LAYOUT          = 32;
	static const int MAX_TEXTURE         = 1024;
	static const int MAX_SHADER          = 64;

private:
	render* m_render;
	
	Texture*      m_texture;
	RenderTarget* m_rt;
	Shader*       m_shader;
	State*        m_state;

}; // RenderContext

}
}

#endif // _UNIRENDER_GL_RENDER_CONTEXT_H_