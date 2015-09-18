#ifndef _EASYPARTICLE2D_TOOLBAR_PANEL_H_
#define _EASYPARTICLE2D_TOOLBAR_PANEL_H_

#include "LoadAdapter.h"

#include <drag2d.h>

namespace eparticle2d
{

class StagePanel;
class ComponentPanel;

class ToolbarPanel : public d2d::ToolbarPanel, public d2d::UICallback
{
public:
	ToolbarPanel(wxWindow* parent, d2d::LibraryPanel* library,
		StagePanel* stage);

	//
	// UICallback interface
	//
	virtual void SetValue(int key, const d2d::UICallback::Data& data);
	virtual void GetValue(int key, d2d::UICallback::Data& data);

	void Load(const Json::Value& val);
	void Store(Json::Value& val) const;

	void Add(const LoadAdapter::Component& comp);

	void InitParticle();

	void OnDelChild(ComponentPanel* child);

protected:
	virtual wxSizer* initLayout();

private:
	wxSizer* CreateMainLayout();
	wxSizer* CreateMainCommonLayout();
	wxSizer* CreateModeGravityLayout();
	wxSizer* CreateModeRadiusLayout();
	wxSizer* CreateModeSpdCosLayout();

	wxSizer* CreateComponentLayout();

	void OnChangeMode(wxCommandEvent& event);

	void OnSetRotationIsDir(wxCommandEvent& event);

private:
	void Clear();

	void OnAddChild(wxCommandEvent& event, d2d::ISymbol* symbol);
	void OnDelAllChild(wxCommandEvent& event);

private:
	class DropTarget : public wxTextDropTarget
	{
	public:
		DropTarget(d2d::LibraryPanel* library, StagePanel* stage,
			ToolbarPanel* toolbar);

		virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& data);

	private:
		d2d::LibraryPanel* m_library;
		StagePanel* m_stage;
		ToolbarPanel* m_toolbar;

	}; // DropTarget 

private:
	StagePanel* m_stage;

	std::vector<d2d::ISliderCtrl*> m_sliders;
	std::vector<std::vector<d2d::ISliderCtrl*> > m_mode_sliders;

	wxChoice* m_mode_choice;

	wxCheckBox* m_rotation_is_dir_ctrl;

	wxSizer* m_main_sizer;

	wxSizer* m_comp_sizer;
	std::vector<ComponentPanel*> m_children;

	friend class FileIO;

}; // ToolbarPanel

}

#endif // _EASYPARTICLE2D_TOOLBAR_PANEL_H_