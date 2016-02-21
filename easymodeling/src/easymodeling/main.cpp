#include "main.h"
#include "Frame.h"

#include <wx/image.h>

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	wxImage::AddHandler(new wxPNGHandler);

	emodeling::Frame* frame = new emodeling::Frame("EasyMoodeling");
	frame->Show(true);

	return true;
}