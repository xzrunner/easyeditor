#ifndef _EASYUI_WRAPPER_TOOLBAR_PANEL_H_
#define _EASYUI_WRAPPER_TOOLBAR_PANEL_H_

#include <drag2d.h>

namespace eui
{
namespace wrapper
{

class ToolbarPanel : public d2d::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent);

	std::string GetType() const;
	void SetType(const std::string& type);

protected:
	virtual wxSizer* initLayout();

private:
	wxTextCtrl* m_type_text;

}; // ToolbarPanel

}
}

#endif // _EASYUI_WRAPPER_TOOLBAR_PANEL_H_