#include "Sprite.h"
#include "Symbol.h"
#include "Node.h"
#include "Database.h"
#include "NodeType.h"

#include <gum/FilepathHelper.h>
#include <gum/Image.h>

namespace edb
{

Sprite::Sprite(int id)
	: m_id(id)
	, m_snapshoot(NULL)
{
}

Sprite::Sprite(const Sprite& spr)
	: s2::Sprite(spr)
	, ee::Sprite(spr)
	, m_id(spr.m_id)
	, m_in_closed(true)
	, m_out_closed(true)
	, m_snapshoot(NULL)
{
}

Sprite& Sprite::operator = (const Sprite& spr)
{
	s2::Sprite::operator = (spr);
	ee::Sprite::operator = (spr);

	m_id = spr.m_id;

	m_in_closed = m_out_closed = true;

	m_snapshoot = NULL;

	return *this;
}

Sprite::Sprite(Symbol* sym, int id)
	: s2::Sprite(sym)
	, ee::Sprite(sym)
	, m_id(id)
	, m_in_closed(true)
	, m_out_closed(true)
	, m_snapshoot(NULL)
{
	const Database* db = sym->GetDB();
	const Node* node = db->Fetch(id);
	if (node->Type() == NODE_LEAF) {
		std::string ss_path = gum::FilepathHelper::Dir(db->GetProjPath()) + "\\" + node->GetPath();
		ss_path = ss_path.substr(0, ss_path.find_last_of(".")) + "_ss.png";
		if (gum::FilepathHelper::Exists(ss_path)) {
			m_snapshoot = gum::ImageMgr::Instance()->Create(ss_path);
		}
	}
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