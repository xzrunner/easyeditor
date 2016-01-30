#ifndef _EASYTEXPACKER_STAGE_PANEL_H_
#define _EASYTEXPACKER_STAGE_PANEL_H_

#include <drag2d.h>
#include <easyphysics.h>

namespace etexpacker
{

class BaseStrategy;

class StagePanel : public d2d::EditPanel, public d2d::SpritesPanelImpl, public ephysics::PhysicsPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame);

	void insertSpriteNoArrange(d2d::Sprite* sprite);

	void arrangeAllSprites(bool bClearSelection);
	void loadFromLibrary();

	int GetTextureAccount() const;

public:
	static void fixCoords(d2d::Sprite* sprite);

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