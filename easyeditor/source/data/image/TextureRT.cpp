#include "TextureRT.h"

#include <sprite2/RenderTarget.h>

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

int TextureRT::GetChannels() const
{
	return 4;
}

void TextureRT::Reload()
{	
}

}