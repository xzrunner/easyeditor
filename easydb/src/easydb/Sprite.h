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
	Sprite(Symbol* sym, int id);

	/**
	 *  @interface
	 *    s2::Sprite
	 */
	virtual Sprite* Clone() const { return new Sprite(*this); }

	int GetID() const { return m_id; }

	void SetClosed(bool in, bool closed);
	bool IsClosed(bool in) const;

	const gum::Image* GetSnapshoot() const { return m_snapshoot; }

private:
	int m_id;

	bool m_in_closed, m_out_closed;

	gum::Image* m_snapshoot;

}; // Sprite

}

#endif // _EASYDB_SPRITE_H_