#ifndef _EASYEDITOR_AUTO_ALIGN_H_
#define _EASYEDITOR_AUTO_ALIGN_H_

#include <SM_Vector.h>

#include <vector>

namespace ee
{

class Sprite;
class MultiSpritesImpl;

class AutoAlign
{
public:
	AutoAlign(MultiSpritesImpl* sprites_impl);

	void Align(const std::vector<Sprite*>& sprites);
	void Align(const Sprite* src, Sprite* dst);

	void Draw() const;

	void SetInvisible();

	bool IsOpen() const { return m_open; }
	void SetOpen(bool open) { m_open = open; }

private:
	MultiSpritesImpl* m_sprites_impl;

	bool m_open;
	sm::vec2 m_hor[2], m_ver[2];

}; // AutoAlign

}

#endif // _EASYEDITOR_AUTO_ALIGN_H_