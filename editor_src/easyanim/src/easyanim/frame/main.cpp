#include "main.h"
#include "frame/MainFrame.h"

#include <wx/image.h>

namespace eanim
{

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	wxImage::AddHandler(new wxPNGHandler);

	MainFrame* frame = new MainFrame(wxT("FruitsAnimator"));
	frame->Show(true);

	return true;
}

} // eanim