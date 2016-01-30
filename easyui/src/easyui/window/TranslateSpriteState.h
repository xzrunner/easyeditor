#ifndef _EASYUI_WINDOW_TRANSLATE_SPRITE_STATE_H_
#define _EASYUI_WINDOW_TRANSLATE_SPRITE_STATE_H_

#include <drag2d.h>

namespace eui
{
namespace window
{

class StagePanel;
class AnchorMgr;

class TranslateSpriteState : public d2d::TranslateSpriteState
{
public:
	TranslateSpriteState(StagePanel* stage, d2d::SpriteSelection* selection, 
		const d2d::Vector& first_pos, AnchorMgr* anchor_mgr);

	virtual void OnMouseRelease(const d2d::Vector& pos);

private:
	class Visitor : public d2d::Visitor
	{
	public:
		Visitor(AnchorMgr* anchor_mgr) : m_anchor_mgr(anchor_mgr) {}
		virtual void Visit(d2d::Object* object, bool& next);
	private:
		AnchorMgr* m_anchor_mgr;
	}; // Visitor

private:
	StagePanel* m_stage;

	AnchorMgr* m_anchor_mgr;

}; // TranslateSpriteState

}
}

#endif // _EASYUI_WINDOW_TRANSLATE_SPRITE_STATE_H_