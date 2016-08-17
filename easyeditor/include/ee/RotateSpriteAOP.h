#ifndef _EASYEDITOR_ROTATE_SPRITE_AOP_H_
#define _EASYEDITOR_ROTATE_SPRITE_AOP_H_

#include "AtomicOP.h"

#include <SM_Vector.h>

namespace ee
{

class Sprite;
class SpriteSelection;

class RotateSpriteAOP : public AtomicOP
{
public:
	RotateSpriteAOP() : m_inited(false) {}

	RotateSpriteAOP(const std::vector<Sprite*>& sprs, const sm::vec2& start, const sm::vec2& end);
	RotateSpriteAOP(const std::vector<Sprite*>& sprs, float angle);
	RotateSpriteAOP(const SpriteSelection& selection, const sm::vec2& start, const sm::vec2& end);
	RotateSpriteAOP(const SpriteSelection& selection, float angle);
	RotateSpriteAOP(Sprite* spr, float angle);
	virtual ~RotateSpriteAOP();

	virtual void Undo();
	virtual void Redo();

	virtual Json::Value Store(const std::vector<Sprite*>& sprs) const;

private:
	void Init(const std::vector<Sprite*>& sprs, const sm::vec2& start, const sm::vec2& end);
	void Init(const std::vector<Sprite*>& sprs, float angle);

private:
	std::vector<Sprite*> m_sprs;

	sm::vec2 m_start, m_end;

	float m_angle; // in rad

	bool m_inited;

}; // RotateSpriteAOP

}

#endif // _EASYEDITOR_ROTATE_SPRITE_AOP_H_