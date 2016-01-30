#include "TextureFBO.h"

//#include "render/FBO.h"

namespace ee
{

TextureFBO::TextureFBO(const FBO* fbo)
	: m_fbo(fbo)
{
//	m_fbo->Retain();
}

TextureFBO::~TextureFBO()
{
//	m_fbo->Release();
}

unsigned int TextureFBO::GetTexID() const 
{ 
//	return m_fbo->GetTexID(); 
	return 0;
}

int TextureFBO::GetWidth() const
{
//	return m_fbo->GetWidth();
	return 0;
}

int TextureFBO::GetHeight() const
{
//	return m_fbo->GetHeight();
	return 0;
}

int TextureFBO::GetChannels() const
{
	return 4;
}

void TextureFBO::Reload()
{	
}

}