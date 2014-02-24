#include "main.h"
#include "frame/Frame.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	eanim::Frame* frame = new eanim::Frame(wxT("easyanim"));
	frame->Show(true);

	if (wxGetApp().argc > 1) {
		d2d::GLCanvas* canvas = frame->getCanvas();
		if (canvas)
			canvas->SetCurrentCanvas();

		wxString path(wxGetApp().argv[1]);
		frame->initWithFile(path);
	}

	return true;
}