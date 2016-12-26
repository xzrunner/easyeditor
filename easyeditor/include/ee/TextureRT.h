#ifndef _EASYEDITOR_TEXTURE_RT_H_
#define _EASYEDITOR_TEXTURE_RT_H_

#include "Texture.h"

namespace ee
{

class RenderTarget;

class TextureRT : public Texture
{
public:
	TextureRT(const RenderTarget* rt);
	virtual ~TextureRT();

	virtual unsigned int GetTexID() const;

	virtual int GetWidth() const;
	virtual int GetHeight() const;
	virtual int GetChannels() const;

	virtual void Reload();

private:
	const RenderTarget* m_rt;

}; // TextureRT

}

#endif // _EASYEDITOR_TEXTURE_RT_H_