
#include "LibraryList.h"
#include "BodyEditStage.h"
#include "Context.h"
#include "StagePanel.h"

using namespace emodeling;

LibraryList::LibraryList(wxWindow* parent)
	: d2d::LibraryList(parent)
{
}

void LibraryList::onListDoubleClicked(wxCommandEvent& event)
{
	libshape::Symbol symbol;
	symbol.SetBG(getSymbol(event.GetInt()));
	libshape::MainDialog<BodyEditStage> dlg(this, &symbol);
	dlg.ShowModal();
	Context::Instance()->stage->resetCanvas();
}
