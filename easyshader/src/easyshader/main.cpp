#include "main.h"
#include "Task.h"

#include <easyshader.h>

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	d2d::Frame* frame = new d2d::Frame("EasyShader", eshader::FILE_TAG);
	eshader::Task* task = new eshader::Task(frame);
	frame->setTask(task);
	frame->Show(true);

	return true;
}