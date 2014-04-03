#include "main.h"
#include "Frame.h"

#include <easyshape.h>

IMPLEMENT_APP(MyApp)

static void InitSymbolCreators() 
{
	d2d::SymbolFactory::RegisterCreator(libshape::FILE_TAG, &libshape::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(libshape::FILE_TAG, &libshape::Sprite::Create);
}

bool MyApp::OnInit()
{
	InitSymbolCreators();

	eshape::Frame* frame = new eshape::Frame(wxT("EasyShape"));
	frame->Show(true);

	return true;
}