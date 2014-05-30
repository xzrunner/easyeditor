#ifndef _SG_STAGE_PANEL_H_
#define _SG_STAGE_PANEL_H_

#include <drag2d.h>

#include "CheckerBoard.h"

namespace sg
{

class ResourceMgr;

class StagePanel : public d2d::EditPanel, public d2d::SpritesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		d2d::LibraryPanel* library);
	virtual ~StagePanel();

	//
	// d2d::EditPanel interface
	//
	virtual void clear();

	//
	// d2d::SpritesPanelImpl interface
	//
	virtual void removeSprite(d2d::ISprite* sprite);
	virtual void insertSprite(d2d::ISprite* sprite);
	virtual void clearSprites();

	void transCoordsToGridPos(const d2d::Vector& pos, int& row, int& col) const;
	void transGridPosToCoords(int row, int col, d2d::Vector& pos) const;

	void updateAllSpritesLocation();

	void getLayoutInfo(int& row, int& col, int& edge) {
		row = m_row;
		col = m_col;
		edge = m_edge;
	}
	void setLayoutInfo(int row, int col, int edge) {
		m_row = row;
		m_col = col;
		m_edge = edge;
	}
	bool getPerspective() const { return m_is_flat; }
	void setPerspective(bool is_flat);

	void changeSpritesLevel(bool up);

	int getLevel() const { return m_level; }
	void setLevel(int level) { m_level = level; }

	ResourceMgr* getResourceMgr() { return m_resource; }

	const CheckerBoard& getCheckBoard() const {
		return m_checkboard;
	}
	CheckerBoard& getCheckBoard() {
		return m_checkboard;
	}

private:
	d2d::Vector fixSpriteLocation(const d2d::Vector& pos) const;

	void changeSymbolRemain(d2d::ISprite* sprite, bool increase) const;

public:
	d2d::ISprite* m_grid;
	d2d::ISymbol *m_arrow_down, *m_arrow_right;

	d2d::ISprite* m_grass[5];

private:
	d2d::LibraryPanel* m_library;

	// data
	int m_row, m_col;
	int m_edge;
	bool m_is_flat;

	int m_level;

	ResourceMgr* m_resource;

	CheckerBoard m_checkboard;

}; // StagePanel

}

#endif // _SG_STAGE_PANEL_H_