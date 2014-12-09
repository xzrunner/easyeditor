#include "App.h"
#include "Frame.h"

namespace rt
{

BEGIN_EVENT_TABLE(App, wxApp)
END_EVENT_TABLE()

IMPLEMENT_APP(App)

bool App::OnInit()
{
	wxInitAllImageHandlers();

	m_frame = new Frame(wxPoint(200,200), wxSize(700,500));
	m_frame->Centre();
	m_frame->Show(TRUE);
	SetTopWindow(m_frame);
	return TRUE;
}

int App::OnExit()
{
	return 0;
}

void App::SetStatusText(const wxString&  text, int number)
{
	m_frame->SetStatusText(text, number);
}

}