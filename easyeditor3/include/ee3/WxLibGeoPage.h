#pragma once

#include <ee0/WxLibraryPage.h>

namespace ee3
{

class WxLibGeoPage : public ee0::WxLibraryPage
{
public:
	WxLibGeoPage(wxWindow* parent);

private:
	void LoadDefaultSymbol();

	void LoadGeometric(const std::string& name);

}; // WxLibGeoPage

}