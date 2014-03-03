
#include "main.h"

#include "Frame/Frame.h"

#include <wx/image.h>

using namespace ebuilder;

IMPLEMENT_APP(MyApp)

const wxString TITLE = wxT("EasyBuilder");

bool MyApp::OnInit()
{
	wxImage::AddHandler(new wxPNGHandler);

	Frame* frame = new Frame(TITLE);
	frame->Show(true);

	return true;
}
