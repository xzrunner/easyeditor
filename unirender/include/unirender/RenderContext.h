#ifndef _UNIRENDER_RENDER_CONTEXT_H_
#define _UNIRENDER_RENDER_CONTEXT_H_

#include "VertexAttrib.h"
#include "typedef.h"

#include <vector>

namespace ur
{

class RenderContext
{
public:
	RenderContext() {}
	virtual ~RenderContext() {}

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
	virtual int  CreateRenderTarget(int id) = 0;
	virtual void ReleaseRenderTarget(int id) = 0;

	virtual void BindRenderTarget(int id) = 0;
	virtual void BindRenderTargetTex(int tex_id) = 0;
	virtual void UnbindRenderTarget() = 0;

	virtual int  CheckRenderTargetStatus() = 0;

// 	virtual void SetCurrRenderTarget(int id) = 0;
// 	virtual int  GetCurrRenderTarget() const = 0;

	/************************************************************************/
	/* Shader                                                               */
	/************************************************************************/

	virtual int  CreateShader(const char* vs, const char* fs) = 0;
	virtual void ReleaseShader(int id) = 0;

	virtual void BindShader(int id) = 0;

	virtual int  GetShaderUniform(const char* name) = 0;
	virtual void SetShaderUniform(int loc, UNIFORM_FORMAT format, const float* v) = 0;

	/************************************************************************/
	/* State                                                                */
	/************************************************************************/

	virtual void EnableBlend(bool blend) = 0;
	virtual void SetBlend(int m1, int m2) = 0;
	virtual void SetBlendEquation(int func) = 0;
	virtual void SetDefaultBlend() = 0;

	virtual void SetClearFlag(int flag) = 0;
	virtual void Clear(unsigned long argb) = 0;

	virtual void EnableScissor(int enable) = 0;
	virtual void SetScissor(int x, int y, int width, int height) = 0;

	virtual void SetViewport(int x, int y, int w, int h) = 0;
	virtual void GetViewport(int& x, int& y, int& w, int& h) = 0;

	virtual void SetDepth(DEPTH_FORMAT d) = 0;

	virtual bool IsTexture(int id) const = 0;

	virtual bool OutOfMemory() const = 0;
	virtual void CheckError() const = 0;

	virtual void SetPointSize(float size) = 0;
	virtual void SetLineWidth(float size) = 0;

	virtual void EnableLineStripple(bool stripple) = 0;
	virtual void SetLineStripple(int pattern) = 0;

	/************************************************************************/
	/* Draw                                                                 */
	/************************************************************************/

	virtual void DrawElements(DRAW_MODE mode, int fromidx, int ni) = 0;
	virtual void DrawArrays(DRAW_MODE mode, int fromidx, int ni) = 0;

	virtual int  CreateBuffer(RENDER_OBJ what, const void *data, int n, int stride) = 0;
	virtual void ReleaseBuffer(RENDER_OBJ what, int id) = 0;
	virtual void BindBuffer(RENDER_OBJ what, int id) = 0;
	virtual void UpdateBuffer(int id, const void* data, int n) = 0;

	virtual int  CreateVertexLayout(const std::vector<VertexAttrib>& va_list) = 0;
	virtual void ReleaseVertexLayout(int id) = 0;
	virtual void BindVertexLayout(int id) = 0;

	virtual void ReadPixels(const void* pixels, int width, int height) = 0;

}; // RenderContext

}

#endif // _UNIRENDER_RENDER_CONTEXT_H_