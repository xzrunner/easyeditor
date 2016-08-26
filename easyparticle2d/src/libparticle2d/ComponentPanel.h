#ifndef _EASYPARTICLE2D_COMPONENT_PANEL_H_
#define _EASYPARTICLE2D_COMPONENT_PANEL_H_

#include <wx/panel.h>

#include <ee/UICallback.h>

#include <ps_2d.h>

#include <vector>

namespace ee { class SliderCtrl; }

class wxButton;

namespace eparticle2d
{

class ToolbarPanel;

class ComponentPanel : public wxPanel, public ee::UICallback
{
public:
	ComponentPanel(wxWindow* parent, p2d_symbol* component, 
		ToolbarPanel* toolbar);

	//
	// UICallback interface
	//
	virtual void SetValue(int key, const ee::UICallback::Data& data);
	virtual void GetValue(int key, ee::UICallback::Data& data);

	void UpdateBtnColor();

private:
	void InitLayout();
	void InitLayout(wxSizer* sizer);

	void OnDelete(wxCommandEvent& event);

	void OnSetBeginMulCol(wxCommandEvent& event);
	void OnSetEndMulCol(wxCommandEvent& event);
	void OnSetBeginAddCol(wxCommandEvent& event);
	void OnSetEndAddCol(wxCommandEvent& event);

	void ChangeColor(uint8_t rgba[4]);

private:
	p2d_symbol* m_pc;

	ToolbarPanel* m_toolbar;

	std::vector<ee::SliderCtrl*> m_sliders;

	wxButton *m_begin_mul_col_btn, *m_end_mul_col_btn;
	wxButton *m_begin_add_col_btn, *m_end_add_col_btn;

	friend class FileIO;
	friend class ToolbarPanel;

}; // ComponentPanel 

}

#endif // _EASYPARTICLE2D_COMPONENT_PANEL_H_