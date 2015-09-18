#ifndef _EASYPARTICLE2D_COMPONENT_PANEL_H_
#define _EASYPARTICLE2D_COMPONENT_PANEL_H_

#include <drag2d.h>
#include <particle2d.h>

namespace eparticle2d
{

class ToolbarPanel;

class ComponentPanel : public wxPanel, public d2d::UICallback
{
public:
	ComponentPanel(wxWindow* parent, p2d_symbol* component, 
		ToolbarPanel* toolbar);

	//
	// UICallback interface
	//
	virtual void SetValue(int key, const d2d::UICallback::Data& data);
	virtual void GetValue(int key, d2d::UICallback::Data& data);

private:
	void InitLayout();
	void InitLayout(wxSizer* sizer);

	void OnDelete(wxCommandEvent& event);

	void OnSetMultiColStart(wxCommandEvent& event);
	void OnSetMultiColEnd(wxCommandEvent& event);
	void OnSetAddColStart(wxCommandEvent& event);
	void OnSetAddColEnd(wxCommandEvent& event);

	void UpdateColor(ps_color4f& color);

private:
	p2d_symbol* m_component;

	ToolbarPanel* m_toolbar;

	std::vector<d2d::ISliderCtrl*> m_sliders;

	friend class FileIO;

}; // ComponentPanel 

}

#endif // _EASYPARTICLE2D_COMPONENT_PANEL_H_