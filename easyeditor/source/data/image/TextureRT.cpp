#include "TextureRT.h"
#include "RenderTarget.h"

namespace ee
{

TextureRT::TextureRT(const RenderTarget* rt)
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
	return m_rt->GetWidth();
}

int TextureRT::GetHeight() const
{
	return m_rt->GetHeight();
}

int TextureRT::GetChannels() const
{
	return 4;
}

void TextureRT::Reload()
{	
}

}