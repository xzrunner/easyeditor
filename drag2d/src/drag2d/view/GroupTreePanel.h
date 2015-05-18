#ifndef _DRAG2D_GROUP_TREE_PANEL_H_
#define _DRAG2D_GROUP_TREE_PANEL_H_

#include <wx/panel.h>

namespace d2d
{

class GroupTreeCtrl;
class MultiSpritesImpl;

class GroupTreePanel : public wxPanel
{
public:
	GroupTreePanel(wxWindow* parent, MultiSpritesImpl* sprites_impl);

private:
	void InitLayout();
	
	wxSizer* InitButtonsLayout();

private:
	void OnAddPress(wxCommandEvent& event);
	void OnDelPress(wxCommandEvent& event);

private:
	GroupTreeCtrl* m_grouptree;

}; // GroupTreePanel

}

#endif // _DRAG2D_GROUP_TREE_PANEL_H_