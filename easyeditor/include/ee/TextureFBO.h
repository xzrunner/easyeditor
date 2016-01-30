#ifndef _EASYEDITOR_TEXTURE_FBO_H_
#define _EASYEDITOR_TEXTURE_FBO_H_

#include "Texture.h"

namespace ee
{

class FBO;

class TextureFBO : public Texture
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

#endif // _EASYEDITOR_TEXTURE_FBO_H_