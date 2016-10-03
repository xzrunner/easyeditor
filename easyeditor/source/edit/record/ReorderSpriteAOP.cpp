#include "ReorderSpriteAOP.h"
#include "Sprite.h"
#include "sprite_msg.h"

namespace ee
{

ReorderSpriteAOP::ReorderSpriteAOP(const std::vector<Sprite*>& sprs, bool up)
	: m_up(up)
{
	for_each(sprs.begin(), sprs.end(), cu::AddRefFunctor<Sprite>());
	m_sprs = sprs;
}

ReorderSpriteAOP::~ReorderSpriteAOP()
{
	for_each(m_sprs.begin(), m_sprs.end(), cu::RemoveRefFunctor<Sprite>());
}

void ReorderSpriteAOP::Undo()
{
	Reorder(!m_up);
}

void ReorderSpriteAOP::Redo()
{
	Reorder(m_up);
}

Json::Value ReorderSpriteAOP::Store(const std::vector<Sprite*>& sprs) const
{
	return Json::Value();
}

void ReorderSpriteAOP::Reorder(bool up)
{
	if (up) {
		for (int i = m_sprs.size() - 1; i >= 0; --i) {
			ReorderSpriteSJ::Instance()->Reorder(m_sprs[i], up);	
		}
	} else {
		for (int i = 0, n = m_sprs.size(); i < n; ++i) {
			ReorderSpriteSJ::Instance()->Reorder(m_sprs[i], up);
		}
	}
}

}