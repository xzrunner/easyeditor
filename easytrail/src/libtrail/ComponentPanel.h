#ifndef _EASYTRAIL_COMPONENT_PANEL_H_
#define _EASYTRAIL_COMPONENT_PANEL_H_

#include <ee/UICallback.h>

#include <wx/panel.h>

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

	//
	// UICallback interface
	//
	virtual void SetValue(int key, const ee::UICallback::Data& data);
	virtual void GetValue(int key, ee::UICallback::Data& data);

	void SetSymbol(t2d_symbol* sym) { m_pc = sym; }

private:
	void InitLayout();
	void InitLayout(wxSizer* sizer);

	void OnDelete(wxCommandEvent& event);

private:
	t2d_symbol* m_pc;

	ToolbarPanel* m_toolbar;

	std::vector<ee::SliderCtrl*> m_sliders;

//	wxButton *m_mul_col_btn, *m_add_col_btn;

}; // ComponentPanel

}

#endif // _EASYTRAIL_COMPONENT_PANEL_H_