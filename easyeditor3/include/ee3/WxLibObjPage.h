#pragma once

#include <ee0/WxLibraryPage.h>

namespace ee3
{
	
class WxLibObjPage : public ee0::WxLibraryPage
{
public:
	WxLibObjPage(wxWindow* parent);

protected:
	virtual void OnAddPress(wxCommandEvent& event) override;

}; // WxLibObjPage

}