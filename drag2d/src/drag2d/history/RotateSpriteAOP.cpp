#include "RotateSpriteAOP.h"
#include "HistoryUtil.h"
#include "AtomicType.h"

#include "common/Math.h"
#include "common/visitors.h"
#include "dataset/Sprite.h"
#include "view/SpriteSelection.h"

namespace d2d
{

RotateSpriteAOP::RotateSpriteAOP(const std::vector<Sprite*>& sprites, const Vector& start, const Vector& end)
{
	Init(sprites, start, end);
}

RotateSpriteAOP::RotateSpriteAOP(const std::vector<Sprite*>& sprites, float angle)
{
	Init(sprites, angle);
}

RotateSpriteAOP::RotateSpriteAOP(const SpriteSelection& selection, const Vector& start, const Vector& end)
{
	std::vector<Sprite*> sprites;
	selection.Traverse(FetchAllVisitor<Sprite>(sprites));
	Init(sprites, start, end);
}

RotateSpriteAOP::RotateSpriteAOP(Sprite* sprite, float angle)
{
	std::vector<Sprite*> sprites;
	sprites.push_back(sprite);
	Init(sprites, angle);
}

RotateSpriteAOP::~RotateSpriteAOP()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->Release();
	}
}

void RotateSpriteAOP::Undo()
{
	if (m_start.IsValid() && m_end.IsValid()) 
	{
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			Sprite* sprite = m_sprites[i];
			float angle = Math2D::GetAngleInDirection(sprite->GetPosition(), m_start, m_end);
			sprite->Rotate(-angle);
		}
	} 
	else if (m_angle != 0) 
	{
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			Sprite* sprite = m_sprites[i];
			sprite->Rotate(-m_angle);
		}
	}
}

void RotateSpriteAOP::Redo()
{
	if (m_start.IsValid() && m_end.IsValid()) 
	{
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			Sprite* sprite = m_sprites[i];
			float angle = Math2D::GetAngleInDirection(sprite->GetPosition(), m_start, m_end);
			sprite->Rotate(angle);
		}
	} 
	else if (m_angle != 0) 
	{
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			Sprite* sprite = m_sprites[i];
			sprite->Rotate(m_angle);
		}
	}
}

Json::Value RotateSpriteAOP::Store(const std::vector<Sprite*>& sprites) const
{
	Json::Value val;
	val["idx"] = HistoryUtil::StoreSpritesIndex(m_sprites, sprites);
	val["type"] = AT_ROTATE;
	val["xstart"] = m_start.x;
	val["ystart"] = m_start.y;
	val["xend"] = m_end.x;
	val["yend"] = m_end.y;
	val["angle"] = m_angle;
	return val;
}

void RotateSpriteAOP::Init(const std::vector<Sprite*>& sprites, 
						   const Vector& start, const Vector& end)
{
	m_start = start;
	m_end = end;
	m_angle = 0;

	for (size_t i = 0, n = sprites.size(); i < n; ++i) 
	{
		sprites[i]->Retain();
		m_sprites.push_back(sprites[i]);
	}
}

void RotateSpriteAOP::Init(const std::vector<Sprite*>& sprites, float angle)
{
	m_start.SetInvalid();
	m_end.SetInvalid();
	m_angle = angle;

	for (size_t i = 0, n = sprites.size(); i < n; ++i) 
	{
		sprites[i]->Retain();
		m_sprites.push_back(sprites[i]);
	}
}

}