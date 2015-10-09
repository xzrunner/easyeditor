#ifndef _EASYTEXPACKER_STAGE_PANEL_H_
#define _EASYTEXPACKER_STAGE_PANEL_H_

#include <drag2d.h>
#include <easyphysics.h>

namespace etp
{

class BaseStrategy;

class StagePanel : public d2d::EditPanel, public d2d::SpritesPanelImpl, public ephysics::PhysicsPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame);

	//
	// d2d::EditPanel interface
	//
	virtual void Clear();

	//
	// d2d::SpritesPanelImpl interface
	//
	virtual bool InsertSprite(d2d::ISprite* sprite, int idx = -1);
	virtual bool RemoveSprite(d2d::ISprite* sprite);

	void insertSpriteNoArrange(d2d::ISprite* sprite);

	void arrangeAllSprites(bool bClearSelection);
	void loadFromLibrary();

	int GetTextureAccount() const;

public:
	static void fixCoords(d2d::ISprite* sprite);

private:
	b2Body* CreateGround();

private:
	BaseStrategy* m_strategy;

}; // StagePanel

}

#endif // _EASYTEXPACKER_STAGE_PANEL_H_