#ifndef _EASYPARTICLE2D_COMPONENT_PANEL_H_
#define _EASYPARTICLE2D_COMPONENT_PANEL_H_

#include <wx/panel.h>

#include <ee/UICallback.h>

#include <ps_2d.h>

#include <vector>

namespace ee { class SliderCtrl; }

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

private:
	void InitLayout();
	void InitLayout(wxSizer* sizer);

	void OnDelete(wxCommandEvent& event);

	void OnSetMultiColStart(wxCommandEvent& event);
	void OnSetMultiColEnd(wxCommandEvent& event);
	void OnSetAddColStart(wxCommandEvent& event);
	void OnSetAddColEnd(wxCommandEvent& event);

	void UpdateColor(ps_color& color);

private:
	p2d_symbol* m_component;

	ToolbarPanel* m_toolbar;

	std::vector<ee::SliderCtrl*> m_sliders;

	friend class FileIO;

}; // ComponentPanel 

}

#endif // _EASYPARTICLE2D_COMPONENT_PANEL_H_