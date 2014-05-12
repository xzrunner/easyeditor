#include "main.h"

#include "Frame.h"

#include <wx/image.h>

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	wxImage::AddHandler(new wxPNGHandler);

	eparticle2d::Frame* frame = new eparticle2d::Frame(wxT("EasyParticle2D"));
	frame->Show(true);

	return true;
}