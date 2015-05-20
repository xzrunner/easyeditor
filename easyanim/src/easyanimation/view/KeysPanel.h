#ifndef _EASYANIM_KEYS_PANEL_H_
#define _EASYANIM_KEYS_PANEL_H_

#include <wx/wx.h>

namespace eanim
{

class Controller;

class KeysPanel : public wxScrolledWindow
{
public:
	KeysPanel(wxWindow* parent, Controller* ctrl);

	void GetSelectPos(int& row, int& col) const;
	void SetSelectPos(int row, int col);

	void GetSelectRegion(int& row, int& col_min, int& col_max);
	void SetSelectRegion(int row, int col);
	void UpdateSelectRegion(int col_min, int col_max);

private:
	void InitLayout();

private:
	Controller* m_ctrl;

	int m_selected_row, m_selected_col;
	int m_selected_col_min, m_selected_col_max;

}; // KeysPanel

}

#endif // _EASYANIM_KEYS_PANEL_H_