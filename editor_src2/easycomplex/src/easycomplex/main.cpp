#include "main.h"

#include <drag2d.h>

#include "Task.h"

IMPLEMENT_APP(MyApp)

// void init_sprites() 
// {
// 	const char* str = " \
// 					  require \"d2d.stage\" \
// 					  sprite = d2d.stage.getSprite() \
// 					  local x, y = sprite.getPosition() \
// 					  ";
// 	d2d::scripts_do_string(str);
// }

extern d2d::StageModule MODULE_STAGE;

bool MyApp::OnInit()
{
	d2d::Frame* frame = new d2d::Frame("EasyComplex", "complex");
	ecomplex::Task* task = new ecomplex::Task(frame);
	frame->setTask(task);
	frame->Show(true);

	if (wxGetApp().argc > 1) {
		wxString path(wxGetApp().argv[1]);
		frame->initWithFile(path);
	}

// 	// sprites
// 	MODULE_STAGE.impl = task->getStagePanel();
// 	init_sprites();
// 	//

	return true;
}