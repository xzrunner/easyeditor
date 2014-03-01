#include "main.h"
#include "Frame.h"

using namespace raiden;

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	Frame* frame = new Frame(wxT("Raiden"));
	frame->Show(true);

	return true;
}