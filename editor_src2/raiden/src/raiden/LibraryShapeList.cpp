#include "LibraryShapeList.h"
#include "Context.h"
#include "StagePanel.h"

#include <easyshape.h>

using namespace raiden;

LibraryShapeList::LibraryShapeList(wxWindow* parent)
	: d2d::LibraryList(parent)
{
}

void LibraryShapeList::onListDoubleClicked(wxCommandEvent& event)
{
	d2d::ISymbol* symbol = getSymbol(event.GetInt());

	{
		libshape::MainDialog<libshape::StagePanel> dlg(this, symbol);
		dlg.ShowModal();
	}

	Context::Instance()->stage->resetCanvas();
	symbol->refresh();
	Context::Instance()->library->Refresh();
	Context::Instance()->stage->resetPathOffset(symbol);
}
