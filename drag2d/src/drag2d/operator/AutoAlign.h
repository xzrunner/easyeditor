#ifndef _DRAG2D_AUTO_ALIGN_H_
#define _DRAG2D_AUTO_ALIGN_H_

#include <vector>

#include "common/Vector.h"

namespace d2d
{

class ISprite;
class MultiSpritesImpl;

class AutoAlign
{
public:
	AutoAlign(MultiSpritesImpl* sprites_impl);

	void Align(const std::vector<ISprite*>& sprites);
	void Align(const ISprite* src, ISprite* dst);

	void Draw() const;

	void SetInvisible();

	bool IsOpen() const { return m_open; }
	void SetOpen(bool open) { m_open = open; }

private:
	MultiSpritesImpl* m_sprites_impl;

	bool m_open;
	Vector m_hor[2], m_ver[2];

}; // AutoAlign

}

#endif // _DRAG2D_AUTO_ALIGN_H_