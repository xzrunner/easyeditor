#include "LibraryImagePage.h"

#include "Context.h"
#include "ToolBarPanel.h"

using namespace formation;

LibraryImagePage::LibraryImagePage(wxWindow* parent)
	: ee::LibraryImagePage(parent, false)
{
}

void LibraryImagePage::onAddPress(wxCommandEvent& event)
{
	ee::LibraryImagePage::onAddPress(event);
	Context::Instance()->toolbar->updateByLibrary();
}

void LibraryImagePage::onDelPress(wxCommandEvent& event)
{
	ee::LibraryImagePage::onDelPress(event);
	Context::Instance()->toolbar->updateByLibrary();
}