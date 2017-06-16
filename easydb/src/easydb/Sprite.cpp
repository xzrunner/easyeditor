#include "Sprite.h"
#include "Symbol.h"

namespace edb
{

Sprite::Sprite(int id)
	: m_id(id)
{
}

Sprite::Sprite(const Sprite& spr)
	: s2::Sprite(spr)
	, ee::Sprite(spr)
	, m_id(spr.m_id)
	, m_in_closed(true)
	, m_out_closed(true)
{
}

Sprite& Sprite::operator = (const Sprite& spr)
{
	s2::Sprite::operator = (spr);
	ee::Sprite::operator = (spr);

	m_id = spr.m_id;

	m_in_closed = m_out_closed = true;

	return *this;
}

Sprite::Sprite(Symbol* sym, int id)
	: s2::Sprite(sym)
	, ee::Sprite(sym)
	, m_id(id)
	, m_in_closed(true)
	, m_out_closed(true)
{
}

void Sprite::SetClosed(bool in, bool closed)
{
	if (in) {
		m_in_closed = closed;
	} else {
		m_out_closed = closed;
	}
}

bool Sprite::IsClosed(bool in) const
{
	return in ? m_in_closed : m_out_closed;
}

}