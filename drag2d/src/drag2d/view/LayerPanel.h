#ifndef _DRAG2D_LAYER_PANEL_H_
#define _DRAG2D_LAYER_PANEL_H_

#include <wx/wx.h>

namespace d2d
{

class LayerList;

class LayerPanel : public wxPanel
{
public:
	LayerPanel(wxWindow* parent);

private:
	void InitLayout();
	wxSizer* InitHeaderLayout();

	void OnAddPress(wxCommandEvent& event);
	void OnDelPress(wxCommandEvent& event);

private:
	wxButton *m_btn_add, *m_btn_del;

	LayerList* m_list;

}; // LayerPanel

}

#endif // _DRAG2D_LAYER_PANEL_H_