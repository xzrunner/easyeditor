#include "main.h"
#include "Frame.h"

using namespace formation;

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	Frame* frame = new Frame(wxT("Formation"));
	frame->Show(true);

	return true;
}