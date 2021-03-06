#ifndef _EASYTEXPACKER_STAGE_PANEL_H_
#define _EASYTEXPACKER_STAGE_PANEL_H_

#include <ee/EditPanel.h>
#include <ee/SpritesPanelImpl.h>
#include <ee/PhysicsPanelImpl.h>

namespace etexpacker
{

class BaseStrategy;

class StagePanel : public ee::EditPanel, public ee::SpritesPanelImpl, public ee::PhysicsPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame);

	void InsertSpriteNoArrange(const ee::SprPtr& spr);

	void ArrangeAllSprites(bool bClearSelection);
	void LoadFromLibrary();

	int GetTextureAccount() const;

public:
	static void FixCoords(const ee::SprPtr& spr);

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud) override;

private:
	b2Body* CreateGround();

private:
	BaseStrategy* m_strategy;

}; // StagePanel

}

#endif // _EASYTEXPACKER_STAGE_PANEL_H_