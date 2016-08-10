#ifndef _EASYTRAIL_COMPONENT_PANEL_H_
#define _EASYTRAIL_COMPONENT_PANEL_H_

#include <ee/UICallback.h>

#include <wx/panel.h>
#include <json/json.h>

#include <vector>

#include <stdint.h>

struct t2d_symbol;

namespace ee { class SliderCtrl; }

namespace etrail
{

class ToolbarPanel;

class ComponentPanel : public wxPanel, public ee::UICallback
{
public:
	ComponentPanel(wxWindow* parent, t2d_symbol* pc, ToolbarPanel* toolbar);
	virtual ~ComponentPanel();

	//
	// UICallback interface
	//
	virtual void SetValue(int key, const ee::UICallback::Data& data);
	virtual void GetValue(int key, ee::UICallback::Data& data);

	virtual void Load(const Json::Value& val, const std::string& dir);
	virtual void Store(Json::Value& val, const std::string& dir) const;

	void SetSymbol(t2d_symbol* sym) { m_pc = sym; }

protected:
	virtual void InitLayout(wxSizer* sizer) = 0;
	virtual std::string GetTitle() const = 0;

	virtual void UpdateBtnColor() = 0;

	void InitLayout();

	void OnDelete(wxCommandEvent& event);

	void ChangeColor(uint8_t rgba[4]);

protected:
	t2d_symbol* m_pc;

	ToolbarPanel* m_toolbar;

	std::vector<ee::SliderCtrl*> m_sliders;

}; // ComponentPanel

}

#endif // _EASYTRAIL_COMPONENT_PANEL_H_