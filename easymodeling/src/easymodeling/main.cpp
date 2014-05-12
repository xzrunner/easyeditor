
#include "main.h"
#include "Frame.h"

#include <wx/image.h>

using namespace emodeling;

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	wxImage::AddHandler(new wxPNGHandler);

	Frame* frame = new Frame(wxT("EasyMoodeling"));
	frame->Show(true);

	return true;
}