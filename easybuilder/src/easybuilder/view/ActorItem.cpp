
#include "ActorItem.h"

#include "dataset/Actor.h"

using namespace ebuilder;

ActorItem::ActorItem(Actor* actor)
	: m_actor(actor)
{
	m_filepath = m_actor->getSymbol().getFilepath();
	const d2d::Bitmap* init = d2d::BitmapMgr::Instance()->getItem(m_filepath);
	if (m_bitmap != init) delete m_bitmap;
	m_bitmap = const_cast<d2d::Bitmap*>(init);
}

void ActorItem::reloadTexture() const
{
}

void ActorItem::draw(const d2d::Colorf& mul, const d2d::Colorf& add,
					 const d2d::ISprite* sprite/* = NULL*/) const
{
//	m_actor->getSymbol().draw();
}

d2d::Rect ActorItem::getSize(const d2d::ISprite* sprite /*= NULL*/) const
{
	return d2d::Rect();
}

void ActorItem::loadResources()
{
}
