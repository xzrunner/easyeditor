#pragma once

#include <ee/LibraryPage.h>

namespace eone
{

class LibGeoPage : public ee::LibraryPage
{
public:
	LibGeoPage(wxWindow* parent);

	virtual bool IsHandleSymbol(const ee::SymPtr& sym) const override;

	virtual void LoadDefaultSymbol() override;

private:
	void LoadGeometric(const std::string& name);

}; // LibGeoPage

}