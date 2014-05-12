#include "LibraryImagePage.h"

#include "Context.h"
#include "ToolBarPanel.h"

using namespace formation;

LibraryImagePage::LibraryImagePage(wxWindow* parent)
	: d2d::LibraryImagePage(parent, false)
{
}

void LibraryImagePage::onAddPress(wxCommandEvent& event)
{
	d2d::LibraryImagePage::onAddPress(event);
	Context::Instance()->toolbar->updateByLibrary();
}

void LibraryImagePage::onDelPress(wxCommandEvent& event)
{
	d2d::LibraryImagePage::onDelPress(event);
	Context::Instance()->toolbar->updateByLibrary();
}