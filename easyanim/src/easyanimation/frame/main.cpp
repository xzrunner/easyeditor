#include "main.h"
#include "frame/Frame.h"

#include <easycomplex.h>
#include <easyanim.h>
#include <easyscale9.h>
#include <easymesh.h>
#include <easyparticle3d.h>

IMPLEMENT_APP(MyApp)

static void InitSymbolCreators() 
{
	d2d::SymbolFactory::RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(eanim::FILE_TAG, &anim::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(eanim::FILE_TAG, &anim::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(escale9::FILE_TAG, &escale9::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(escale9::FILE_TAG, &escale9::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(emesh::FILE_TAG, &emesh::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(emesh::FILE_TAG, &emesh::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(eparticle3d::FILE_TAG, &eparticle3d::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(eparticle3d::FILE_TAG, &eparticle3d::Sprite::Create);
}

bool MyApp::OnInit()
{
	InitSymbolCreators();

	eanim::Frame* frame = new eanim::Frame(wxT("easyanim"));
	frame->Show(true);

	if (wxGetApp().argc > 1) {
		d2d::GLCanvas* canvas = frame->getCanvas();
		if (canvas)
			canvas->SetCurrentCanvas();

		wxString path(wxGetApp().argv[1]);
		frame->initWithFile(path);
	}

	return true;
}