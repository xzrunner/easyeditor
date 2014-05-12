
#include "Dialog.h"
#include "Panel.h"
#include "Adapter.h"
#include "AnimationControl.h"

#include "dataset/Game.h"

using namespace ebuilder::preview;

Dialog::Dialog(wxWindow* parent, ebuilder::Scene* scene, 
			   ebuilder::LibraryPanel* libraryPanel)
	: wxDialog(parent, wxID_ANY, wxT("Preview"), wxDefaultPosition, 
	wxSize(Game::WIDTH, Game::HEIGHT), wxCLOSE_BOX | wxCAPTION)
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	Panel* panel = new Panel(this, this, libraryPanel);

	AnimationControl::Instance()->registerCanvas(panel->getCanvas());
	AnimationControl::Instance()->start();

	Adapter adapter(panel);
	panel->setScene(adapter.builder2Scene(*scene));

	sizer->Add(panel, 1, wxEXPAND);
	SetSizer(sizer);
}

Dialog::~Dialog()
{
	AnimationControl::Instance()->stop();
	AnimationControl::Instance()->clear();
}
