#ifndef EPACKER_STAGE_PANEL_H
#define EPACKER_STAGE_PANEL_H

#include <drag2d.h>
#include <easyphysics.h>

namespace epacker
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
	virtual bool InsertSprite(d2d::ISprite* sprite);
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

#endif // EPACKER_STAGE_PANEL_H