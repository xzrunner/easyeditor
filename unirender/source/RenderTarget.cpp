#include "RenderTarget.h"
#include "RenderContext.h"
#include "Texture.h"

namespace ur
{

RenderTarget::RenderTarget(RenderContext* rc, int width, int height)
	: m_rc(rc)
{
	m_texture = new Texture(rc, width, height);
	m_id = m_rc->CreateRenderTarget(0);
}

RenderTarget::~RenderTarget()
{
	m_rc->ReleaseRenderTarget(m_id);
	delete m_texture;
}

void RenderTarget::Bind()
{
	m_rc->BindRenderTarget(m_id);
	m_rc->BindRenderTargetTex(m_texture->ID());
}

void RenderTarget::Unbind()
{
	m_rc->UnbindRenderTarget();
}

}