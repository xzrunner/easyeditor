#ifndef _UNIRENDER_RENDER_CONTEXT_H_
#define _UNIRENDER_RENDER_CONTEXT_H_

namespace ur
{

class IRenderContext
{
public:
	struct Callback
	{
		void (*state_change)();
	};

public:
	IRenderContext(const Callback& cb);
	virtual ~IRenderContext() {}

	virtual int RenderVersion() const = 0;

	/************************************************************************/
	/* Texture                                                              */
	/************************************************************************/

	virtual int  CreateTexture(const void* pixels, int width, int height, int format) = 0;
	virtual void ReleaseTexture(int id) = 0;

	virtual void UpdateTexture(const void* pixels, int w, int h, unsigned int id) = 0;
	virtual void UpdateSubTexture(const void* pixels, int x, int y, int w, int h, unsigned int id) = 0;

	virtual void BindTexture(int id, int channel) = 0;

	virtual void ClearTextureCache() = 0;

	virtual int  GetCurrTexture() const = 0;

	/************************************************************************/
	/* RenderTarget                                                         */
	/************************************************************************/
	virtual int  CreateRT(int id) = 0;
	virtual void ReleaseRT(int id) = 0;

	virtual void BindRT(int id) = 0;
	virtual void BindRTTexture(int tex_id) = 0;

	virtual int  CheckRTStatus() = 0;

	virtual void SetCurrRT(int id) = 0;
	virtual int  GetCurrRT() const = 0;

	/************************************************************************/
	/* Shader                                                               */
	/************************************************************************/

	virtual int  CreateShader(const char* vs, const char* fs) = 0;
	virtual void ReleaseShader(int id) = 0;

	virtual void BindShader(int id) = 0;

	/************************************************************************/
	/* State                                                                */
	/************************************************************************/

	virtual void SetBlend(int m1, int m2) = 0;
	virtual void SetBlendEquation(int func) = 0;
	virtual void SetDefaultBlend() = 0;

	virtual void SetClearFlag(int flag) = 0;
	virtual void Clear(unsigned long argb) = 0;

	virtual void EnableScissor(int enable) = 0;
	virtual void SetScissor(int x, int y, int width, int height) = 0;

	virtual void SetViewport(int x, int y, int w, int h) = 0;
	virtual void GetViewport(int& x, int& y, int& w, int& h) = 0;

}; // IRenderContext

}

#endif // _UNIRENDER_RENDER_CONTEXT_H_