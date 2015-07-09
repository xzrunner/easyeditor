#include "MirrorSpriteAOP.h"
#include "AtomicType.h"
#include "HistoryUtil.h"

#include "dataset/ISprite.h"

namespace d2d
{

MirrorSpriteAOP::MirrorSpriteAOP(ISprite* sprite, 
								 bool old_mirror_x, bool old_mirror_y,
								 bool new_mirror_x, bool new_mirror_y)
	: m_sprite(sprite)
	, m_old_mirror_x(old_mirror_x)
	, m_old_mirror_y(old_mirror_y)
	, m_new_mirror_x(new_mirror_x)
	, m_new_mirror_y(new_mirror_y)
{
	m_sprite->Retain();
}

MirrorSpriteAOP::~MirrorSpriteAOP()
{
	m_sprite->Release();
}

void MirrorSpriteAOP::undo()
{
	m_sprite->SetMirror(m_old_mirror_x, m_old_mirror_y);
}

void MirrorSpriteAOP::redo()
{
	m_sprite->SetMirror(m_new_mirror_x, m_new_mirror_y);
}

Json::Value MirrorSpriteAOP::store(const std::vector<ISprite*>& sprites)
{
	Json::Value val;
	val["type"] = AT_SHEAR;
	val["new_mirror_x"] = m_new_mirror_x;
	val["new_mirror_y"] = m_new_mirror_y;
	val["old_mirror_x"] = m_old_mirror_x;
	val["old_mirror_y"] = m_old_mirror_y;
	return val;
}

}