
#include "ActorItem.h"

#include "dataset/Actor.h"

using namespace ebuilder;

ActorItem::ActorItem(Actor* actor)
	: m_actor(actor)
{
	m_filepath = m_actor->getSymbol().getFilepath();
	const ee::Bitmap* init = ee::BitmapMgr::Instance()->getItem(m_filepath);
	if (m_bitmap != init) delete m_bitmap;
	m_bitmap = const_cast<ee::Bitmap*>(init);
}

void ActorItem::reloadTexture() const
{
}

void ActorItem::draw(const ee::Matrix& mt,
					 const ee::Colorf& mul, 
					 const ee::Colorf& add,
					 const ee::ISprite* sprite/* = NULL*/) const
{
//	m_actor->getSymbol().draw();
}

ee::Rect ActorItem::getSize(const ee::ISprite* sprite /*= NULL*/) const
{
	return ee::Rect();
}

void ActorItem::loadResources()
{
}
