#include "main.h"
#include "Frame.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	edb::Frame* frame = new edb::Frame(wxT("EasyDB"));
	frame->Show(true);

	return true;
}