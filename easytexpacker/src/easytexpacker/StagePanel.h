#ifndef _EASYTEXPACKER_STAGE_PANEL_H_
#define _EASYTEXPACKER_STAGE_PANEL_H_


#include <easyphysics.h>

namespace etexpacker
{

class BaseStrategy;

class StagePanel : public ee::EditPanel, public ee::SpritesPanelImpl, public ephysics::PhysicsPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame);

	void insertSpriteNoArrange(ee::Sprite* sprite);

	void arrangeAllSprites(bool bClearSelection);
	void loadFromLibrary();

	int GetTextureAccount() const;

public:
	static void fixCoords(ee::Sprite* sprite);

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	b2Body* CreateGround();

private:
	BaseStrategy* m_strategy;

}; // StagePanel

}

#endif // _EASYTEXPACKER_STAGE_PANEL_H_