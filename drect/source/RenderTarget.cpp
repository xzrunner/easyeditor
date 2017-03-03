#include "RenderTarget.h"

#include <unirender/UR_RenderTarget.h>
#include <unirender/UR_Texture.h>

namespace drect
{

RenderTarget::RenderTarget(ur::RenderContext* rc, int width, int height, int extend)
	: m_cx(0)
	, m_cy(0)
	, m_width(width)
	, m_height(height)
	, m_extend(extend)
{
	m_rt = new ur::RenderTarget(rc, width + extend * 2, height + extend * 2);
}

RenderTarget::~RenderTarget()
{
	delete m_rt;
}

void RenderTarget::Bind()
{
	m_rt->Bind();
}

void RenderTarget::Unbind()
{
	m_rt->Unbind();
}

void RenderTarget::SetCenter(int x, int y)
{
	m_cx = x;
	m_cy = y;
}

int RenderTarget::GetTexID() const
{
	return m_rt->TexID();
}

}