
#include "main.h"
#include "Frame.h"

using namespace eshape;

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	Frame* frame = new Frame(wxT("EasyShape"));
	frame->Show(true);

	return true;
}