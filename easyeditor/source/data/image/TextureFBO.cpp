#include "TextureFBO.h"
#include "FBO.h"

namespace ee
{

TextureFBO::TextureFBO(const FBO* fbo)
	: m_fbo(fbo)
{
	m_fbo->Retain();
}

TextureFBO::~TextureFBO()
{
	m_fbo->Release();
}

unsigned int TextureFBO::GetTexID() const 
{ 
	return m_fbo->GetTexID(); 
}

int TextureFBO::GetWidth() const
{
	return m_fbo->GetWidth();
}

int TextureFBO::GetHeight() const
{
	return m_fbo->GetHeight();
}

int TextureFBO::GetChannels() const
{
	return 4;
}

void TextureFBO::Reload()
{	
}

}