#pragma once

#include <wx/wx.h>
#include <drag2d.h>

namespace eanim
{

class Symbol;

class PreviewDialog : public wxDialog
{
public:
	PreviewDialog(wxWindow* parent, const Symbol* symbol);
	~PreviewDialog();

private:
	void initLayout();

	void buildToolBar(wxSizer* topSizer);
	void buildEditPanel(wxSizer* topSizer);

	void onSetCirculate(wxCommandEvent& event);
	void onSetStop(wxCommandEvent& event);

private:
	d2d::EditPanel* m_stage;

	const Symbol* m_symbol;

}; // PreviewDialog

}

