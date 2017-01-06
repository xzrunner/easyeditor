#ifndef _EASYUI_WINDOW_ARRANGE_SPRITE_IMPL_H_
#define _EASYUI_WINDOW_ARRANGE_SPRITE_IMPL_H_

#include <ee/ArrangeSpriteImpl.h>

namespace eui
{
namespace window
{

class StagePanel;

class ArrangeSpriteImpl : public ee::ArrangeSpriteImpl
{
public:
	ArrangeSpriteImpl(StagePanel* stage, ee::PropertySettingPanel* property);

	virtual void OnMouseLeftDown(int x, int y);
	virtual void OnMouseLeftUp(int x, int y);

	virtual void OnDraw(float cam_scale) const;

private:
	virtual ee::ArrangeSpriteState* CreateTranslateState(ee::SpriteSelection* selection, 
		const sm::vec2& first_pos) const;

private:
	StagePanel* m_stage;

	mutable float m_center_node_radius;

	bool m_move_center;
	mutable ee::Sprite* m_selected;

}; // ArrangeSpriteImpl

}
}

#endif // _EASYUI_WINDOW_ARRANGE_SPRITE_IMPL_H_