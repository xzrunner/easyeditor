#pragma once

#include "ee0/WxLibraryPage.h"

namespace ee0
{

class WxLibraryImagePage : public WxLibraryPage
{
public:
	WxLibraryImagePage(wxWindow* parent);

protected:
	virtual void OnAddPress(wxCommandEvent& event) override;

}; // WxLibraryImagePage

}