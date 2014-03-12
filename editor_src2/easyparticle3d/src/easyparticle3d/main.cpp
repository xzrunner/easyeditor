#include "main.h"

#include "Frame.h"
#include "CocoPacker.h"

#include <wx/image.h>

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	wxImage::AddHandler(new wxPNGHandler);

	eparticle3d::Frame* frame = new eparticle3d::Frame(wxT("EasyParticle3D"));
	frame->Show(true);

	if (wxGetApp().argc > 3) 
	{
		wxString cmd(wxGetApp().argv[1]);
		wxString dir(wxGetApp().argv[2]);
		wxString output(wxGetApp().argv[3]);
		eparticle3d::CocoPacker::pack(dir, output);
	}

	return true;
}