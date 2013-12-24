#include "main.h"
#include "frame/Frame.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	eanim::Frame* frame = new eanim::Frame(wxT("easyanim"));
	frame->Show(true);

	return true;
}