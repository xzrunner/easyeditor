#include "main.h"
#include "Task.h"

#include <easypseudo3d.h>

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	d2d::Frame* frame = new d2d::Frame("EasyPseudo3D", epseudo3d::FILE_TAG);
	epseudo3d::Task* task = new epseudo3d::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	return true;
}