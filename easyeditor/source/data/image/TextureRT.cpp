#include "TextureRT.h"

#include <gimg_typedef.h>
#include <sprite2/S2_RenderTarget.h>

namespace ee
{

TextureRT::TextureRT(const s2::RenderTarget* rt)
	: m_rt(rt)
{
	m_rt->AddReference();
}

TextureRT::~TextureRT()
{
	m_rt->RemoveReference();
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
	return GPF_RGBA;
}

}