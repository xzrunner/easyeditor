
#include "LibraryList.h"
#include "BodyEditStage.h"
#include "Context.h"
#include "StagePanel.h"

using namespace emodeling;

LibraryList::LibraryList(ee::LibraryPage* page)
	: ee::LibraryList(page)
{
}

void LibraryList::OnListDoubleClicked(wxCommandEvent& event)
{
	// todo

// 	eshape::Symbol symbol;
// 	symbol.SetBG(getSymbol(event.GetInt()));
// 	eshape::EditDialog<BodyEditStage> dlg(this, &symbol);
// 	dlg.ShowModal();
// 	Context::Instance()->stage->resetCanvas();
}
