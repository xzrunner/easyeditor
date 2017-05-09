#ifndef _EASYEDITOR_EDIT_SPR_MSG_H_
#define _EASYEDITOR_EDIT_SPR_MSG_H_

#include <SM_Vector.h>
#include <sprite2/Color.h>

namespace ee
{

class Sprite;
class SpriteSelection;

class EditSprMsg
{
public:
	static void Rotate(const SpriteSelection& selection, float rotate);
	static void Translate(const SpriteSelection& selection, const sm::vec2& offset);

	static void SetScale(const Sprite* spr, const sm::vec2& pos, const sm::vec2& scale);

	static void SetColMul(const Sprite* spr, const s2::Color& mul);
	static void SetColAdd(const Sprite* spr, const s2::Color& add);

}; // EditSprMsg

}

#endif // _EASYEDITOR_EDIT_SPR_MSG_H_