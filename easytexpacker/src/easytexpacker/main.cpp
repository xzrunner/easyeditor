#include "main.h"
#include "Frame.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	etexpacker::Frame* frame = new etexpacker::Frame("EasyPacker");
	frame->Show(true);

	return true;
}