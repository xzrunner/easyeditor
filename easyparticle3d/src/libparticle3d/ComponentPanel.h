#ifndef _EASYPARTICLE3D_COMPONENT_PANEL_H_
#define _EASYPARTICLE3D_COMPONENT_PANEL_H_

#include <ee/UICallback.h>

#include <wx/panel.h>

#include <vector>

#include <stdint.h>

namespace ee { class SliderCtrl; }

struct p3d_symbol;
struct ps_color;
class wxTextCtrl;
class wxButton;

namespace eparticle3d
{

class ToolbarPanel;

class ComponentPanel : public wxPanel, public ee::UICallback
{
public:
	ComponentPanel(wxWindow* parent, p3d_symbol* pc, ToolbarPanel* toolbar);

	//
	// UICallback interface
	//
	virtual void SetValue(int key, const ee::UICallback::Data& data);
	virtual void GetValue(int key, ee::UICallback::Data& data);

	void UpdateBtnColor();

	void SetCount(int count);

private:
	void InitLayout();
	void InitLayout(wxSizer* sizer);

	void OnDelete(wxCommandEvent& event);

	void OnSetCount(wxSpinEvent& event);

	void OnSetBeginMulCol(wxCommandEvent& event);
	void OnSetEndMulCol(wxCommandEvent& event);
	void OnSetBeginAddCol(wxCommandEvent& event);
	void OnSetEndAddCol(wxCommandEvent& event);

	void ChangeColor(uint8_t rgba[4]);

private:
	p3d_symbol* m_pc;

	ToolbarPanel* m_toolbar;

	std::vector<ee::SliderCtrl*> m_sliders;

	wxTextCtrl* m_name;

	wxSpinCtrl* m_count;

	wxButton *m_begin_mul_col_btn, *m_end_mul_col_btn;
	wxButton *m_begin_add_col_btn, *m_end_add_col_btn;

	friend class FileIO;
	friend class ToolbarPanel;

}; // ComponentPanel

}

#endif // _EASYPARTICLE3D_COMPONENT_PANEL_H_