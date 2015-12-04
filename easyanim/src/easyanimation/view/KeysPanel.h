#ifndef _EASYANIM_KEYS_PANEL_H_
#define _EASYANIM_KEYS_PANEL_H_

#include <drag2d.h>

namespace eanim
{

class KeysPanel : public wxScrolledWindow, public d2d::Observer
{
public:
	KeysPanel(wxWindow* parent);

	void GetSelectPos(int& row, int& col) const;

	void GetSelectRegion(int& row, int& col_min, int& col_max);

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	void InitLayout();

	void SetSelectPos(int row, int col);

	void SetSelectRegion(int row, int col);
	void UpdateSelectRegion(int col_min, int col_max);

private:
	int m_selected_row, m_selected_col;
	int m_selected_col_min, m_selected_col_max;

}; // KeysPanel

}

#endif // _EASYANIM_KEYS_PANEL_H_