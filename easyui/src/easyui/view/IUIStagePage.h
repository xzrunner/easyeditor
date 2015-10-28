#ifndef _EASYUI_I_UI_STAGE_PAGE_H_
#define _EASYUI_I_UI_STAGE_PAGE_H_

#include <drag2d.h>

namespace eui
{

class IUIStagePage : public d2d::EditPanel
{
public:
	IUIStagePage(wxWindow* parent, wxTopLevelWindow* frame)
		: d2d::EditPanel(parent, frame) {}
	virtual void OnSelected() = 0;
	virtual void LoadFromFile(const char* filename) = 0;
	virtual void StoreToFile(const char* filename) const = 0;
}; // IUIStagePage

}

#endif // _EASYUI_I_UI_STAGE_PAGE_H_