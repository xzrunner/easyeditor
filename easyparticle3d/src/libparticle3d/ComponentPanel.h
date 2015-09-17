#ifndef _EASYPARTICLE3D_COMPONENT_PANEL_H_
#define _EASYPARTICLE3D_COMPONENT_PANEL_H_

#include <drag2d.h>

struct p3d_symbol;
struct ps_color4f;

namespace eparticle3d
{

class ToolbarPanel;

class ComponentPanel : public wxPanel, public d2d::UICallback
{
public:
	ComponentPanel(wxWindow* parent, p3d_symbol* pc, ToolbarPanel* toolbar);

	//
	// UICallback interface
	//
	virtual void SetValue(int key, const d2d::UICallback::Data& data);
	virtual void GetValue(int key, d2d::UICallback::Data& data);

	const ps_color4f& GetMulColor() const;
	const ps_color4f& GetAddColor() const;

private:
	void InitLayout();
	void InitLayout(wxSizer* sizer);

	void OnDelete(wxCommandEvent& event);

	void OnBindPS(wxCommandEvent& event);

	void OnSetMultiCol(wxCommandEvent& event);
	void OnSetAddCol(wxCommandEvent& event);

private:
	p3d_symbol* m_pc;

	ToolbarPanel* m_toolbar;

	std::vector<d2d::ISliderCtrl*> m_sliders;

	wxTextCtrl* m_name;

	friend class FileIO;
	friend class ToolbarPanel;

}; // ComponentPanel

}

#endif // _EASYPARTICLE3D_COMPONENT_PANEL_H_