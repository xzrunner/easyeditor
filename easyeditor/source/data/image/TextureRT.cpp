#include "TextureRT.h"

#include <gimg_typedef.h>
#include <sprite2/RenderTarget.h>

namespace ee
{

TextureRT::TextureRT(const std::shared_ptr<const s2::RenderTarget>& rt)
	: m_rt(rt)
{
}

unsigned int TextureRT::GetTexID() const 
{ 
	return m_rt->GetTexID(); 
}

int TextureRT::GetWidth() const
{
	return m_rt->Width();
}

int TextureRT::GetHeight() const
{
	return m_rt->Height();
}

int TextureRT::GetFormat() const
{
	return GPF_RGBA8;
}

}