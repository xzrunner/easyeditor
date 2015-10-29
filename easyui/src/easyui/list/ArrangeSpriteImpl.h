#ifndef _EASYUI_LIST_ARRANGE_SPRITE_IMPL_H_
#define _EASYUI_LIST_ARRANGE_SPRITE_IMPL_H_

#include <drag2d.h>

namespace eui
{
namespace list
{

class StagePanel;

class ArrangeSpriteImpl : public d2d::ArrangeSpriteImpl
{
public:
	ArrangeSpriteImpl(StagePanel* stage, d2d::PropertySettingPanel* property);

private:
	virtual d2d::IArrangeSpriteState* CreateTranslateState(d2d::SpriteSelection* selection, 
		const d2d::Vector& first_pos) const;

private:
	StagePanel* m_stage;

}; // ArrangeSpriteImpl

}
}

#endif // _EASYUI_LIST_ARRANGE_SPRITE_IMPL_H_