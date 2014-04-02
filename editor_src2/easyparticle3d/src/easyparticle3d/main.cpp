#include "main.h"

#include "Frame.h"
#include "CocoPacker.h"

#include <easycomplex.h>
#include <easyanim.h>
#include <easyscale9.h>

#include <wx/image.h>

IMPLEMENT_APP(MyApp)

static void InitSymbolCreators() 
{
	d2d::SymbolFactory::RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(eanim::FILE_TAG, &anim::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(eanim::FILE_TAG, &anim::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(escale9::FILE_TAG, &escale9::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(escale9::FILE_TAG, &escale9::Sprite::Create);
}

bool MyApp::OnInit()
{
	InitSymbolCreators();

	eparticle3d::Frame* frame = new eparticle3d::Frame(wxT("EasyParticle3D"));
	frame->Show(true);

	if (wxGetApp().argc > 3) 
	{
		wxString cmd(wxGetApp().argv[1]);
		wxString dir(wxGetApp().argv[2]);
		wxString output(wxGetApp().argv[3]);
		eparticle3d::CocoPacker::pack(dir, output);
	}

	return true;
}