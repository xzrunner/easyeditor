#ifndef _EASYUI_LIST_ARRANGE_SPRITE_IMPL_H_
#define _EASYUI_LIST_ARRANGE_SPRITE_IMPL_H_



namespace eui
{
namespace list
{

class StagePanel;

class ArrangeSpriteImpl : public ee::ArrangeSpriteImpl
{
public:
	ArrangeSpriteImpl(StagePanel* stage, ee::PropertySettingPanel* property);

private:
	virtual ee::ArrangeSpriteState* CreateTranslateState(ee::SpriteSelection* selection, 
		const ee::Vector& first_pos) const;

private:
	StagePanel* m_stage;

}; // ArrangeSpriteImpl

}
}

#endif // _EASYUI_LIST_ARRANGE_SPRITE_IMPL_H_