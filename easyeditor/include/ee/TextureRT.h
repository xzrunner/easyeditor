#ifndef _EASYEDITOR_TEXTURE_RT_H_
#define _EASYEDITOR_TEXTURE_RT_H_

#include "Texture.h"

#include <memory>

namespace s2 { class RenderTarget; }

namespace ee
{

class TextureRT : public Texture
{
public:
	TextureRT(const std::shared_ptr<const s2::RenderTarget>& rt);

	virtual unsigned int GetTexID() const;

	virtual int GetWidth() const override;
	virtual int GetHeight() const override;
	virtual int GetFormat() const override;

private:
	std::shared_ptr<const s2::RenderTarget> m_rt;

}; // TextureRT

}

#endif // _EASYEDITOR_TEXTURE_RT_H_