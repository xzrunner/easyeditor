#include "main.h"
#include "frame/Frame.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	eanim::Frame* frame = new eanim::Frame(wxT("easyanim"));
	frame->Show(true);

	if (wxGetApp().argc > 1) {
		wxString path(wxGetApp().argv[1]);
		frame->initWithFile(path);
	}

	return true;
}