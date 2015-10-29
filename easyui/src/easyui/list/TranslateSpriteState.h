#ifndef _EASYUI_LIST_TRANSLATE_SPRITE_STATE_H_
#define _EASYUI_LIST_TRANSLATE_SPRITE_STATE_H_

#include <drag2d.h>

namespace eui
{
namespace list
{

class UIList;

class TranslateSpriteState : public d2d::TranslateSpriteState
{
public:
	TranslateSpriteState(d2d::SpriteSelection* selection, 
		const d2d::Vector& first_pos, UIList* list);

protected:
	virtual void Translate(const d2d::Vector& offset);

private:
	UIList* m_list;

}; // TranslateSpriteState

}
}

#endif // _EASYUI_LIST_TRANSLATE_SPRITE_STATE_H_