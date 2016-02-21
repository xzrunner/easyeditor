#ifndef _EASYUI_UI_STAGE_PAGE_H_
#define _EASYUI_UI_STAGE_PAGE_H_

#include <ee/EditPanel.h>

namespace eui
{

class UIStagePage : public ee::EditPanel
{
public:
	UIStagePage(wxWindow* parent, wxTopLevelWindow* frame)
		: ee::EditPanel(parent, frame) {}

	virtual void LoadFromFile(const char* filename) = 0;
	virtual void StoreToFile(const char* filename) const = 0;

	virtual void EnablePage(bool enable) = 0;

	virtual void OnPreview() = 0;
	virtual void OnCode() const = 0;

}; // UIStagePage

}

#endif // _EASYUI_UI_STAGE_PAGE_H_