#include "LibraryShapeList.h"
#include "Context.h"
#include "StagePanel.h"

#include <easyshape.h>

using namespace raiden;

LibraryShapeList::LibraryShapeList(wxWindow* parent)
	: ee::LibraryList(parent)
{
}

void LibraryShapeList::onListDoubleClicked(wxCommandEvent& event)
{
	ee::ISymbol* symbol = getSymbol(event.GetInt());

	{
		libshape::MainDialog<libshape::StagePanel> dlg(this, symbol);
		dlg.ShowModal();
	}

	Context::Instance()->stage->resetCanvas();
	symbol->refresh();
	Context::Instance()->library->Refresh();
	Context::Instance()->stage->resetPathOffset(symbol);
}
