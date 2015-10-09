#ifndef _EASYTEXPACKER_REGULAR_TOOLBAR_PAGE_H_
#define _EASYTEXPACKER_REGULAR_TOOLBAR_PAGE_H_

#include <drag2d.h>

namespace etexpacker
{

class StagePanel;

class RegularToolbarPage : public wxPanel
{
public:
	RegularToolbarPage(wxWindow* parent, StagePanel* stage);

private:
	void InitLayout();

	void OnSetSrcDir(wxCommandEvent& event);

private:
	wxTextCtrl* m_src_dir;

	StagePanel* m_stage;

}; // RegularToolbarPage

}

#endif // _EASYTEXPACKER_REGULAR_TOOLBAR_PAGE_H_