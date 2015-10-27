#ifndef _EASYUI_OVERALL_ARRANGE_SPRITE_IMPL_H_
#define _EASYUI_OVERALL_ARRANGE_SPRITE_IMPL_H_

#include <drag2d.h>

namespace eui
{
namespace overall
{

class StagePanel;

class ArrangeSpriteImpl : public d2d::ArrangeSpriteImpl
{
public:
	ArrangeSpriteImpl(StagePanel* stage, d2d::PropertySettingPanel* property);

private:
	virtual d2d::IArrangeSpriteState* CreateTransalteState(d2d::SpriteSelection* selection, 
		const d2d::Vector& first_pos) const;

private:
	StagePanel* m_stage;

}; // ArrangeSpriteImpl

}
}

#endif // _EASYUI_OVERALL_ARRANGE_SPRITE_IMPL_H_