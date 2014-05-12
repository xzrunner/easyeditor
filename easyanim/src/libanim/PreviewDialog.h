#pragma once

#include <wx/wx.h>
#include <drag2d.h>

namespace anim
{

class Symbol;

class PreviewDialog : public wxDialog
{
public:
	PreviewDialog(wxWindow* parent, d2d::LibraryPanel* library,
		const Symbol* symbol);
	~PreviewDialog();

private:
	void initLayout();

	void buildToolBar(wxSizer* topSizer);
	void buildEditPanel(wxSizer* topSizer);

	void onSetCirculate(wxCommandEvent& event);
	void onSetStop(wxCommandEvent& event);

private:
	d2d::EditPanel* m_stage;
	d2d::LibraryPanel* m_library;

	const Symbol* m_symbol;

}; // PreviewDialog

}

