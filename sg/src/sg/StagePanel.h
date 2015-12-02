#ifndef _SG_STAGE_PANEL_H_
#define _SG_STAGE_PANEL_H_

#include <drag2d.h>

#include "BuildingCfg.h"
#include "CheckerBoard.h"

namespace sg
{

class BuildingCfg;

class StagePanel : public d2d::EditPanel, public d2d::SpritesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		d2d::LibraryPanel* library);

	void TransCoordsToGridPos(const d2d::Vector& pos, int& row, int& col) const;
	void TransGridPosToCoords(int row, int col, d2d::Vector& pos) const;

	void TransCoordsToGridPosNew(const d2d::Vector& pos, int& row, int& col) const;
	void TransGridPosToCoordsNew(int row, int col, d2d::Vector& pos) const;

	void UpdateAllSpritesLocation();

	void GetLayoutInfo(int& row, int& col, int& edge) {
		row = m_row;
		col = m_col;
		edge = m_edge;
	}
	void SetLayoutInfo(int row, int col, int edge) {
		m_row = row;
		m_col = col;
		m_edge = edge;
	}
	bool GetPerspective() const { return m_is_flat; }
	void SetPerspective(bool is_flat);

	void ChangeSelectedSpritesLevel(bool up);

	int GetBaseLevel() const { return m_base_level; }
	void SetBaseLevel(int level) { m_base_level = level; }

	const BuildingCfg& GetBuildingCfg() const { return m_building_cfg; }
	BuildingCfg& GetBuildingCfg() { return m_building_cfg; }

	const CheckerBoard& GetCheckBoard() const { return m_checkboard; }
	CheckerBoard& GetCheckBoard() { return m_checkboard; }

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	d2d::Vector FixSpriteLocation(const d2d::Vector& pos) const;

	void ChangeSymbolRemain(d2d::ISprite* sprite, bool increase) const;

	void Insert(d2d::ISprite* spr);
	void Remove(d2d::ISprite* spr);
	void Clear();

private:
	d2d::LibraryPanel* m_library;

	// layout
	int m_row, m_col;
	int m_edge;
	bool m_is_flat;

	int m_base_level;

	BuildingCfg m_building_cfg;

	CheckerBoard m_checkboard;

}; // StagePanel

}

#endif // _SG_STAGE_PANEL_H_