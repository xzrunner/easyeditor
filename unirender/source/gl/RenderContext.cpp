#include "gl/RenderContext.h"
#include "gl/Texture.h"
#include "gl/RenderTarget.h"
#include "gl/Shader.h"
#include "gl/State.h"
#include "gl/Draw.h"
#include "gl/typedef.h"

#include <ejoy2d/render.h>

#include <stdlib.h>

namespace ur
{
namespace gl
{

RenderContext::RenderContext(const IRenderContext::Callback& cb)
	: IRenderContext(cb)
{
	render_init_args RA;
	// todo: config these args
	RA.max_buffer  = 128;
	RA.max_layout  = MAX_LAYOUT;
	RA.max_target  = 128;
	RA.max_texture = MAX_TEXTURE;
	RA.max_shader  = MAX_SHADER;

	int smz = render_size(&RA);
	m_render = (render*)malloc(smz);
	m_render = render_init(&RA, m_render, smz);

	m_texture = new Texture(m_render, cb.state_change);
	m_rt      = new RenderTarget();
	m_shader  = new Shader(m_render);
	m_state   = new State(m_render, cb.state_change);
	m_draw    = new Draw(m_render);
}

RenderContext::~RenderContext()
{
	render_exit(m_render);
	free(m_render);
}

int RenderContext::RenderVersion() const
{
	return render_version(m_render);
}

/************************************************************************/
/* Texture                                                              */
/************************************************************************/

int  RenderContext::CreateTexture(const void* pixels, int width, int height, int format)
{
	return m_texture->Create(pixels, width, height, format);
}

void RenderContext::ReleaseTexture(int id)
{
	m_texture->Release(id);
}

void RenderContext::UpdateTexture(const void* pixels, int w, int h, unsigned int id)
{
	m_texture->Update(pixels, w, h, id);
}

void RenderContext::UpdateSubTexture(const void* pixels, int x, int y, int w, int h, unsigned int id)
{
	m_texture->UpdateSub(pixels, x, y, w, h, id);
}

void RenderContext::BindTexture(int id, int channel)
{
	m_texture->Bind(id, channel);
}

void RenderContext::ClearTextureCache()
{
	m_texture->ClearCache();
}

int  RenderContext::GetCurrTexture() const
{
	return m_texture->GetCurr();
}

/************************************************************************/
/* RenderTarget                                                         */
/************************************************************************/

int  RenderContext::CreateRT(int id)
{
	return m_rt->Create(id);
}

void RenderContext::ReleaseRT(int id)
{
	m_rt->Release(id);
}

void RenderContext::BindRT(int id)
{
	m_rt->BindTarget(id);
}

void RenderContext::BindRTTexture(int tex_id)
{
	m_rt->BindTexture(tex_id);
}

int  RenderContext::CheckRTStatus()
{
	return m_rt->CheckStatus();
}

void RenderContext::SetCurrRT(int id)
{
	m_rt->SetCurr(id);
}

int  RenderContext::GetCurrRT() const
{
	return m_rt->GetCurr();
}

/************************************************************************/
/* Shader                                                               */
/************************************************************************/

int  RenderContext::CreateShader(const char* vs, const char* fs)
{
	return m_shader->Create(vs, fs);
}

void RenderContext::ReleaseShader(int id)
{
	m_shader->Release(id);
}

void RenderContext::BindShader(int id)
{
	m_shader->Bind(id);
}

int RenderContext::GetShaderUniform(const char* name)
{
	return m_shader->GetUniform(name);
}

void RenderContext::SetShaderUniform(int loc, UNIFORM_FORMAT format, const float* v)
{
	m_shader->SetUniform(loc, format, v);
}

/************************************************************************/
/* State                                                                */
/************************************************************************/

void RenderContext::SetBlend(int m1, int m2)
{
	m_state->SetBlend(m1, m2);
}

void RenderContext::SetBlendEquation(int func)
{
	m_state->SetBlendEquation(func);
}

void RenderContext::SetDefaultBlend()
{
	m_state->SetDefaultBlend();
}

void RenderContext::SetClearFlag(int flag)
{
	m_state->SetClearFlag(flag);
}

void RenderContext::Clear(unsigned long argb)
{
	m_state->Clear(argb);
}

void RenderContext::EnableScissor(int enable)
{
	m_state->EnableScissor(enable);
}

void RenderContext::SetScissor(int x, int y, int width, int height)
{
	m_state->SetScissor(x, y, width, height);
}

void RenderContext::SetViewport(int x, int y, int w, int h)
{
	m_state->SetViewport(x, y, w, h);
}

void RenderContext::GetViewport(int& x, int& y, int& w, int& h)
{
	m_state->GetViewport(x, y, w, h);
}

void RenderContext::SetDepth(DEPTH_FORMAT d)
{
	m_state->SetDepth(d);
}

/************************************************************************/
/* Draw                                                                 */
/************************************************************************/

void RenderContext::DrawElements(DRAW_MODE mode, int fromidx, int ni)
{
	m_draw->DrawElements(mode, fromidx, ni);
}

void RenderContext::DrawArrays(DRAW_MODE mode, int fromidx, int ni)
{
	m_draw->DrawArrays(mode, fromidx, ni);	
}

int  RenderContext::CreateBuffer(RENDER_OBJ what, const void *data, int n, int stride)
{
	return m_draw->CreateBuffer(what, data, n, stride);
}

void RenderContext::ReleaseBuffer(RENDER_OBJ what, int id)
{
	m_draw->ReleaseBuffer(what, id);
}

void RenderContext::BindBuffer(RENDER_OBJ what, int id)
{
	m_draw->BindBuffer(what, id);
}

void RenderContext::UpdateBuffer(int id, const void* data, int n)
{
	m_draw->UpdateBuffer(id, data, n);
}

int  RenderContext::CreateVertexLayout(const std::vector<VertexAttrib>& va_list)
{
	return m_draw->CreateVertexLayout(va_list);
}

void RenderContext::ReleaseVertexLayout(int id)
{
	m_draw->ReleaseVertexLayout(id);
}

void RenderContext::BindVertexLayout(int id)
{
	m_draw->BindVertexLayout(id);
}

}
}