
#include "main.h"

#include "Frame.h"

#include <wx/image.h>

using namespace eparticle;

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	wxImage::AddHandler(new wxPNGHandler);

	Frame* frame = new Frame(wxT("EasyParticle"));
	frame->Show(true);

	return true;
}