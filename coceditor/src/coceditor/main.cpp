#include "main.h"
#include "Frame.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	coceditor::Frame* frame = new coceditor::Frame(wxT("COCEditor"));
	frame->Show(true);

	return true;
}