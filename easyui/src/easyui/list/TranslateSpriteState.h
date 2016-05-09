#ifndef _EASYUI_LIST_TRANSLATE_SPRITE_STATE_H_
#define _EASYUI_LIST_TRANSLATE_SPRITE_STATE_H_

#include <ee/TranslateSpriteState.h>

namespace eui
{
namespace list
{

class UIList;

class TranslateSpriteState : public ee::TranslateSpriteState
{
public:
	TranslateSpriteState(ee::SpriteSelection* selection, 
		const sm::vec2& first_pos, UIList* list);

protected:
	virtual void Translate(const sm::vec2& offset);

private:
	UIList* m_list;

}; // TranslateSpriteState

}
}

#endif // _EASYUI_LIST_TRANSLATE_SPRITE_STATE_H_