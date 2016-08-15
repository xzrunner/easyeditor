#ifndef _EASYUI_WINDOW_TRANSLATE_SPRITE_STATE_H_
#define _EASYUI_WINDOW_TRANSLATE_SPRITE_STATE_H_

#include <ee/TranslateSpriteState.h>

namespace eui
{
namespace window
{

class StagePanel;
class AnchorMgr;

class TranslateSpriteState : public ee::TranslateSpriteState
{
public:
	TranslateSpriteState(StagePanel* stage, ee::SpriteSelection* selection, 
		const sm::vec2& first_pos, AnchorMgr* anchor_mgr);

	virtual void OnMouseRelease(const sm::vec2& pos);

private:
	class Visitor : public ee::Visitor<ee::Sprite>
	{
	public:
		Visitor(AnchorMgr* anchor_mgr) : m_anchor_mgr(anchor_mgr) {}
		virtual void Visit(ee::Sprite* spr, bool& next);
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