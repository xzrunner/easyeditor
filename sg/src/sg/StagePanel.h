#ifndef _SG_STAGE_PANEL_H_
#define _SG_STAGE_PANEL_H_

#include <drag2d.h>

namespace sg
{

class StagePanel : public d2d::EditPanel, public d2d::SpritesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		d2d::LibraryPanel* library);

	//
	// d2d::EditPanel interface
	//
	virtual void clear();

	//
	// d2d::SpritesPanelImpl interface
	//
	virtual void insertSprite(d2d::ISprite* sprite);

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

private:
	d2d::Vector fixSpriteLocation(const d2d::Vector& pos) const;

private:
	// data
	int m_row, m_col;
	int m_edge;
	bool m_is_flat;

}; // StagePanel

}

#endif // _SG_STAGE_PANEL_H_