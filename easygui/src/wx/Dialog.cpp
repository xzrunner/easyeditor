#ifdef EGUI_WX

#include "interface/Dialog.h"
#include "interface/Window.h"

#include <wx/window.h>
#include <wx/dialog.h>

namespace egui
{

//////////////////////////////////////////////////////////////////////////
// class DialogImpl
//////////////////////////////////////////////////////////////////////////

class DialogImpl : public wxDialog
{
public:
	DialogImpl(wxWindow* parent, wxWindowID id, const wxString& title, int w, int h) 
		: wxDialog(parent, id, title, wxDefaultPosition, wxSize(w, h))
	{}

}; // DialogImpl

//////////////////////////////////////////////////////////////////////////
// class Dialog
//////////////////////////////////////////////////////////////////////////

Dialog::Dialog(Window* parent, const std::string& title, int w, int h)
{
	wxWindow* raw_wnd = static_cast<wxWindow*>(parent->GetRawPtr());
	m_impl = new DialogImpl(raw_wnd, wxID_ANY, title, w, h);
}

Dialog::~Dialog()
{
	delete m_impl;
}

void Dialog::ShowModal()
{
	m_impl->ShowModal();
}

}

#endif // EGUI_WX