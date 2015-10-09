#include "main.h"
#include "Frame.h"

using namespace etexpacker;

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	Frame* frame = new Frame(wxT("EasyPacker"));
	frame->Show(true);

	return true;
}