#include "main.h"
#include "Frame.h"

#include <easyshape.h>

IMPLEMENT_APP(MyApp)

static void InitSymbolCreators() 
{
	ee::SymbolFactory::RegisterCreator(libshape::FILE_TAG, &libshape::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(libshape::FILE_TAG, &libshape::Sprite::Create);
}

bool MyApp::OnInit()
{
	InitSymbolCreators();

	raiden::Frame* frame = new raiden::Frame(wxT("Raiden"));
	frame->Show(true);

	return true;
}