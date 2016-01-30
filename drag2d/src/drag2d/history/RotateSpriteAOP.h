#ifndef _DRAG2D_ROTATE_SPRITE_AOP_H_
#define _DRAG2D_ROTATE_SPRITE_AOP_H_

#include "AbstractAtomicOP.h"

#include "common/Vector.h"

namespace d2d
{

class Sprite;
class SpriteSelection;

class RotateSpriteAOP : public AbstractAtomicOP
{
public:
	RotateSpriteAOP() {}

	RotateSpriteAOP(const std::vector<Sprite*>& sprites, const Vector& start, const Vector& end);
	RotateSpriteAOP(const std::vector<Sprite*>& sprites, float angle);
	RotateSpriteAOP(const SpriteSelection& selection, const Vector& start, const Vector& end);
	RotateSpriteAOP(Sprite* sprite, float angle);
	virtual ~RotateSpriteAOP();

	virtual void Undo();
	virtual void Redo();

	virtual Json::Value Store(const std::vector<Sprite*>& sprites) const;

private:
	void Init(const std::vector<Sprite*>& sprites, const Vector& start, const Vector& end);
	void Init(const std::vector<Sprite*>& sprites, float angle);

private:
	std::vector<Sprite*> m_sprites;

	Vector m_start, m_end;

	float m_angle; // in rad

}; // RotateSpriteAOP

}

#endif // _DRAG2D_ROTATE_SPRITE_AOP_H_