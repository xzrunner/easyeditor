#ifndef _EASYDB_SPRITE_H_
#define _EASYDB_SPRITE_H_

#include <ee/Sprite.h>

namespace gum { class Image; }

namespace edb
{

class Symbol;

class Sprite : public ee::Sprite
{
public:
	Sprite(int id);
	Sprite(const Sprite& spr);
	Sprite& operator = (const Sprite& spr);
	Sprite(const std::shared_ptr<Symbol>& sym, int id);

	int GetID() const { return m_id; }

	void SetClosed(bool in, bool closed);
	bool IsClosed(bool in) const;

	const gum::Image* GetSnapshoot() const { return m_snapshoot; }

private:
	int m_id;

	bool m_in_closed, m_out_closed;

	gum::Image* m_snapshoot;

	S2_SPR_CLONE_FUNC(Sprite)

}; // Sprite

}

#endif // _EASYDB_SPRITE_H_