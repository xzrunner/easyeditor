#ifndef _EPACKER_REGULAR_TOOLBAR_PAGE_H_
#define _EPACKER_REGULAR_TOOLBAR_PAGE_H_

#include <drag2d.h>

namespace epacker
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

#endif // _EPACKER_REGULAR_TOOLBAR_PAGE_H_