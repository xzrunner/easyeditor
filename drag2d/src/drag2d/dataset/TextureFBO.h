#ifndef _DRAG2D_TEXTURE_FBO_H_
#define _DRAG2D_TEXTURE_FBO_H_

#include "ITexture.h"

namespace d2d
{

class FBO;

class TextureFBO : public ITexture
{
public:
	TextureFBO(const FBO* fbo);
	virtual ~TextureFBO();

	virtual unsigned int GetTexID() const;

	virtual int GetWidth() const;
	virtual int GetHeight() const;
	virtual int GetChannels() const;

	virtual void Reload();

private:
	const FBO* m_fbo;

}; // TextureFBO

}

#endif // _DRAG2D_TEXTURE_FBO_H_