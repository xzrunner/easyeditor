#ifndef _EASYTRAIL_COMPONENT_PANEL_H_
#define _EASYTRAIL_COMPONENT_PANEL_H_

#include <ee/UICallback.h>

#include <wx/panel.h>
#include <json/json.h>

#include <vector>

struct t2d_symbol;

namespace ee { class SliderCtrl; }

namespace etrail
{

class ToolbarPanel;

class ComponentPanel : public wxPanel, public ee::UICallback
{
public:
	ComponentPanel(wxWindow* parent, t2d_symbol* pc, ToolbarPanel* toolbar);

	virtual void Load(const Json::Value& val);
	virtual void Store(Json::Value& val) const;

	void SetSymbol(t2d_symbol* sym) { m_pc = sym; }

protected:
	virtual void InitLayout(wxSizer* sizer) = 0;
	virtual std::string GetTitle() const = 0;

	void InitLayout();

	void OnDelete(wxCommandEvent& event);

protected:
	t2d_symbol* m_pc;

	ToolbarPanel* m_toolbar;

	std::vector<ee::SliderCtrl*> m_sliders;

}; // ComponentPanel

}

#endif // _EASYTRAIL_COMPONENT_PANEL_H_