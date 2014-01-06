#include "ArrangeSpriteAtomicOP.h"

#include "common/Math.h"
#include "view/MultiSpritesImpl.h"

namespace d2d
{
namespace arrange_sprite
{
//////////////////////////////////////////////////////////////////////////
// class SpritesAOP
//////////////////////////////////////////////////////////////////////////

SpritesAOP::SpritesAOP(const SpriteSelection& selection)
{
	selection.traverse(FetchAllVisitor<ISprite>(m_sprites));
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		m_sprites[i]->retain();
}

SpritesAOP::SpritesAOP(const std::vector<ISprite*>& sprites)
{
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		sprites[i]->retain();
		m_sprites.push_back(sprites[i]);
	}
}

SpritesAOP::~SpritesAOP()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		m_sprites[i]->release();
}

//////////////////////////////////////////////////////////////////////////
// class MoveSpritesAOP
//////////////////////////////////////////////////////////////////////////

MoveSpritesAOP::MoveSpritesAOP(const SpriteSelection& selection, const Vector& offset)
	: SpritesAOP(selection)
{
	m_offset = offset;
}

MoveSpritesAOP::MoveSpritesAOP(const std::vector<ISprite*>& sprites, const Vector& offset)
	: SpritesAOP(sprites)
{
	m_offset = offset;
}

void MoveSpritesAOP::undo()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
	{
		ISprite* sprite = m_sprites[i];
		sprite->translate(-m_offset);
	}
}

void MoveSpritesAOP::redo()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
	{
		ISprite* sprite = m_sprites[i];
		sprite->translate(m_offset);
	}
}

//////////////////////////////////////////////////////////////////////////
// class RotateSpritesAOP
//////////////////////////////////////////////////////////////////////////

RotateSpritesAOP::RotateSpritesAOP(const SpriteSelection& selection, const Vector& start, const Vector& end)
	: SpritesAOP(selection)
	, m_start(start)
	, m_end(end)
	, m_angle(0)
{
}

RotateSpritesAOP::RotateSpritesAOP(const std::vector<ISprite*>& sprites, const Vector& start, const Vector& end)
	: SpritesAOP(sprites)
	, m_start(start)
	, m_end(end)
	, m_angle(0)
{
}

RotateSpritesAOP::RotateSpritesAOP(const std::vector<ISprite*>& sprites, float angle)
	: SpritesAOP(sprites)
	, m_angle(angle)
{
	m_start.setInvalid();
	m_end.setInvalid();
}

void RotateSpritesAOP::undo()
{
	if (m_start.isValid() && m_end.isValid()) {
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			ISprite* sprite = m_sprites[i];
			float angle = Math::getAngleInDirection(sprite->getPosition(), m_start, m_end);
			sprite->rotate(-angle);
		}
	} else if (m_angle != 0) {
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			ISprite* sprite = m_sprites[i];
			sprite->rotate(-m_angle);
		}
	}
}

void RotateSpritesAOP::redo()
{
	if (m_start.isValid() && m_end.isValid()) {
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			ISprite* sprite = m_sprites[i];
			float angle = Math::getAngleInDirection(sprite->getPosition(), m_start, m_end);
			sprite->rotate(angle);
		}
	} else if (m_angle != 0) {
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			ISprite* sprite = m_sprites[i];
			sprite->rotate(m_angle);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// class DeleteSpritesAOP
//////////////////////////////////////////////////////////////////////////

DeleteSpritesAOP::DeleteSpritesAOP(const SpriteSelection& selection, MultiSpritesImpl* spritesImpl)
	: SpritesAOP(selection)
{
	m_spritesImpl = spritesImpl;
}

DeleteSpritesAOP::DeleteSpritesAOP(const std::vector<ISprite*>& sprites, MultiSpritesImpl* spritesImpl)
	: SpritesAOP(sprites)
{
	m_spritesImpl = spritesImpl;
}

void DeleteSpritesAOP::undo()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		m_spritesImpl->insertSprite(m_sprites[i]);
}

void DeleteSpritesAOP::redo()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		m_spritesImpl->removeSprite(m_sprites[i]);
}

//////////////////////////////////////////////////////////////////////////
// class ScaleSpritesAOP
//////////////////////////////////////////////////////////////////////////

ScaleSpritesAOP::ScaleSpritesAOP(const std::vector<ISprite*>& sprites, float xScale, float yScale)
	: SpritesAOP(sprites)
	, m_xScale(xScale)
	, m_yScale(yScale)
{
	m_oldScales.reserve(sprites.size());
	for (size_t i = 0, n = sprites.size(); i < n; ++i) {
		ISprite* sprite = sprites[i];
		m_oldScales.push_back(std::make_pair(sprite->getScaleX(), sprite->getScaleY()));
	}
}

void ScaleSpritesAOP::undo()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i) {
		ISprite* sprite = m_sprites[i];
		sprite->setScale(m_oldScales[i].first, m_oldScales[i].second);
	} 
}

void ScaleSpritesAOP::redo()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i) {
		ISprite* sprite = m_sprites[i];
		sprite->setScale(m_xScale, m_yScale);
	} 
}

//////////////////////////////////////////////////////////////////////////
// class ShearSpritesAOP
//////////////////////////////////////////////////////////////////////////

ShearSpritesAOP::ShearSpritesAOP(const std::vector<ISprite*>& sprites, float xShear, float yShear)
	: SpritesAOP(sprites)
	, m_xShear(xShear)
	, m_yShear(yShear)
{
	m_oldShears.reserve(sprites.size());
	for (size_t i = 0, n = sprites.size(); i < n; ++i) {
		ISprite* sprite = sprites[i];
		m_oldShears.push_back(std::make_pair(sprite->getShearX(), sprite->getShearY()));
	}
}

void ShearSpritesAOP::undo()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i) {
		ISprite* sprite = m_sprites[i];
		sprite->setShear(m_oldShears[i].first, m_oldShears[i].second);
	} 
}

void ShearSpritesAOP::redo()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i) {
		ISprite* sprite = m_sprites[i];
		sprite->setShear(m_xShear, m_yShear);
	} 
}

//////////////////////////////////////////////////////////////////////////
// class ShearSpritesAOP
//////////////////////////////////////////////////////////////////////////

MirrorSpritesAOP::MirrorSpritesAOP(const std::vector<ISprite*>& sprites, bool xMirror, bool yMirror)
	: SpritesAOP(sprites)
	, m_xMirror(xMirror)
	, m_yMirror(yMirror)
{
	m_oldMirrors.reserve(sprites.size());
	for (size_t i = 0, n = sprites.size(); i < n; ++i) {
		ISprite* sprite = sprites[i];
		bool mx, my;
		sprite->getMirror(mx, my);
		m_oldMirrors.push_back(std::make_pair(mx, my));
	}
}

void MirrorSpritesAOP::undo()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i) {
		ISprite* sprite = m_sprites[i];
		sprite->setMirror(m_oldMirrors[i].first, m_oldMirrors[i].second);
	} 
}

void MirrorSpritesAOP::redo()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i) {
		ISprite* sprite = m_sprites[i];
		sprite->setMirror(m_xMirror, m_yMirror);
	} 
}


} // arrange_sprite
} // d2d