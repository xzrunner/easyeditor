#ifndef _UNIRENDER_GL_RENDER_CONTEXT_H_
#define _UNIRENDER_GL_RENDER_CONTEXT_H_

#include "../RenderContext.h"

struct render;

namespace ur
{
namespace gl
{

class RenderContext : public ur::RenderContext
{
public:
	struct Callback
	{
		void (*flush_shader)();
		void (*flush_render_shader)();
	};

public:
	RenderContext(const RenderContext::Callback& cb, int max_texture);
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

	virtual int  CreateRenderTarget(int id);
	virtual void ReleaseRenderTarget(int id);

	virtual void BindRenderTarget(int id);
	virtual void BindRenderTargetTex(int tex_id);
	virtual void UnbindRenderTarget();

	virtual int  CheckRenderTargetStatus();

// 	virtual void SetCurrRenderTarget(int id);
// 	virtual int  GetCurrRenderTarget() const;

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

	virtual void EnableBlend(bool blend);
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

	virtual bool IsTexture(int id) const;

	virtual bool OutOfMemory() const;
	virtual void CheckError() const;

	virtual void SetPointSize(float size);
	virtual void SetLineWidth(float size);

	virtual void EnableLineStripple(bool stripple);
	virtual void SetLineStripple(int pattern);

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

	virtual void ReadPixels(const void* pixels, int width, int height);

private:
	static const int MAX_TEXTURE_CHANNEL = 8;
	static const int MAX_RENDER_TARGET_LAYER = 8;

private:
	render* m_render;
	Callback m_cb;

	/************************************************************************/
	/* Texture                                                              */
	/************************************************************************/

	int m_textures[MAX_TEXTURE_CHANNEL];

	/************************************************************************/
	/* RenderTarget                                                         */
	/************************************************************************/

	int m_rt_depth;
	int m_rt_layers[MAX_RENDER_TARGET_LAYER];

//	int m_curr_rt;

	/************************************************************************/
	/* State                                                                */
	/************************************************************************/

	BLEND_FORMAT m_blend_src, m_blend_dst;
	BLEND_FUNC   m_blend_func;

	int          m_clear_mask;

	int          m_vp_x, m_vp_y, m_vp_w, m_vp_h;

}; // RenderContext

}
}

#endif // _UNIRENDER_GL_RENDER_CONTEXT_H_