#include "main.h"
#include "Task.h"

#include <ee/Frame.h>

#include <easypseudo3d.h>

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	ee::Frame* frame = new ee::Frame("EasyPseudo3D", epseudo3d::FILE_TAG);
	epseudo3d::Task* task = new epseudo3d::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	return true;
}