#pragma once

#include <ee/LibraryPage.h>

namespace ee3
{
	
class LibObjPage : public ee::LibraryPage
{
public:
	LibObjPage(wxWindow* parent);

	virtual bool IsHandleSymbol(const ee::SymPtr& sym) const override;

protected:
	virtual void OnAddPress(wxCommandEvent& event) override;

}; // LibObjPage

}