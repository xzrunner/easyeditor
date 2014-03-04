#include "main.h"

#include "Frame.h"

#include <wx/image.h>

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	wxImage::AddHandler(new wxPNGHandler);

	eparticle3d::Frame* frame = new eparticle3d::Frame(wxT("EasyParticle3D"));
	frame->Show(true);

	return true;
}