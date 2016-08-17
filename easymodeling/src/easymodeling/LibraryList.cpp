#include "LibraryList.h"
#include "BodyEditStage.h"
#include "Context.h"
#include "StagePanel.h"

namespace emodeling
{

LibraryList::LibraryList(ee::LibraryPage* page)
	: ee::LibraryList(page)
{
}

void LibraryList::OnListDoubleClicked(wxCommandEvent& event)
{
	// todo

// 	eshape::Symbol sym;
// 	sym.SetBG(getSymbol(event.GetInt()));
// 	eshape::EditDialog<BodyEditStage> dlg(this, &sym);
// 	dlg.ShowModal();
// 	Context::Instance()->stage->resetCanvas();
}

}