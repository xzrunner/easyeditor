#include "main.h"
#include "Task.h"

#include <ee/Frame.h>
#include <ee/SymbolFile.h>

#include <easytexture.h>

#include <sprite2/SymType.h>

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	ee::Frame* frame = new ee::Frame("EasyTexture", ee::SymbolFile::Instance()->Tag(s2::SYM_TEXTURE));
	etexture::Task* task = new etexture::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	return true;
}