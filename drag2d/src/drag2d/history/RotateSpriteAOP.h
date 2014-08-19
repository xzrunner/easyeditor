#ifndef _DRAG2D_ROTATE_SPRITE_STATE_H_
#define _DRAG2D_ROTATE_SPRITE_STATE_H_

#include "AbstractAtomicOP.h"

#include "dataset/SelectionSet.h"

namespace d2d
{

class RotateSpriteAOP : public AbstractAtomicOP
{
public:
	RotateSpriteAOP() {}

	RotateSpriteAOP(const std::vector<ISprite*>& sprites, const Vector& start, const Vector& end);
	RotateSpriteAOP(const std::vector<ISprite*>& sprites, float angle);
	RotateSpriteAOP(const SpriteSelection& selection, const Vector& start, const Vector& end);
	RotateSpriteAOP(ISprite* sprite, float angle);
	virtual ~RotateSpriteAOP();

	virtual void undo();
	virtual void redo();

	virtual Json::Value store(const std::vector<ISprite*>& sprites);

private:
	void Init(const std::vector<ISprite*>& sprites, const Vector& start, const Vector& end);
	void Init(const std::vector<ISprite*>& sprites, float angle);

private:
	std::vector<ISprite*> m_sprites;

	Vector m_start, m_end;

	float m_angle; // in rad

}; // RotateSpriteAOP

}

#endif // _DRAG2D_ROTATE_SPRITE_STATE_H_