#include "main.h"
#include "Frame.h"

using namespace shootbubble;

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	Frame* frame = new Frame(wxT("ShootBubble"));
	frame->Show(true);

	return true;
}