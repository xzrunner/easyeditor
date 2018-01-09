#pragma once

#include <ee/LibraryPage.h>

namespace enode3d
{
	
class LibraryPage : public ee::LibraryPage
{
public:
	LibraryPage(wxWindow* parent, const std::string& title = "model3d");

	virtual bool IsHandleSymbol(const ee::SymPtr& sym) const override;

protected:
	virtual void OnAddPress(wxCommandEvent& event) override;

}; // LibraryPage

}