
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
	libshape::MainDialog<BodyEditStage> dlg(this, getSymbol(event.GetInt()));
	dlg.ShowModal();
	Context::Instance()->stage->resetCanvas();
}
