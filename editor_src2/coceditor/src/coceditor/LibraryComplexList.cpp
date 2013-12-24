#include "LibraryComplexList.h"
#include "StagePanel.h"
#include "Context.h"

#include "complex/Dialog.h"

namespace coceditor
{

LibraryComplexList::LibraryComplexList(wxWindow* parent)
	: d2d::LibraryList(parent, wxT("complex"))
{
}

void LibraryComplexList::onListDoubleClicked(wxCommandEvent& event)
{
	complex::Dialog dlg(this, static_cast<d2d::ComplexSymbol*>(m_items[event.GetInt()]), 
		Context::Instance()->property);
	dlg.ShowModal();
// 	if (m_editPanel)
// 		m_editPanel->resetCanvas();
	Context::Instance()->stage->resetCanvas();
}

} // coceditor