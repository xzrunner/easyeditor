#ifndef _EASYPARTICLE2D_TOOLBAR_PANEL_H_
#define _EASYPARTICLE2D_TOOLBAR_PANEL_H_

#include <ee/ToolbarPanel.h>
#include <ee/UICallback.h>
#include <ee/SliderCtrl.h>

#include <gum/P2dSymLoader.h>

#include <wx/dnd.h>

namespace ee { class LibraryPanel; }
namespace s2 { class Symbol; }

namespace eparticle2d
{

class StagePanel;
class ComponentPanel;

class ToolbarPanel : public ee::ToolbarPanel, public ee::UICallback
{
public:
	ToolbarPanel(wxWindow* parent, ee::LibraryPanel* library,
		StagePanel* stage);

	//
	// UICallback interface
	//
	virtual void SetValue(int key, const ee::UICallback::Data& data);
	virtual void GetValue(int key, ee::UICallback::Data& data);

	void Load(const Json::Value& val);
	void Store(Json::Value& val) const;

	void Add(const gum::P2dSymLoader::Component& comp);

	void InitParticle();

	void OnDelChild(ComponentPanel* child);

protected:
	virtual wxSizer* InitLayout();

private:
	wxSizer* CreateMainLayout();
	wxSizer* CreateMainCommonLayout();
	wxSizer* CreateModeGravityLayout();
	wxSizer* CreateModeRadiusLayout();
	wxSizer* CreateModeSpdCosLayout();

	wxSizer* CreateComponentLayout();

	void OnSetLoop(wxCommandEvent& event);
	void OnSetLocalModeDraw(wxCommandEvent& event);

	void OnChangeMode(wxCommandEvent& event);

	void OnSetRotationIsDir(wxCommandEvent& event);

private:
	void Clear();

	void OnAddChild(wxCommandEvent& event, s2::Symbol* sym);
	void OnDelAllChild(wxCommandEvent& event);

private:
	class DropTarget : public wxTextDropTarget
	{
	public:
		DropTarget(ee::LibraryPanel* library, StagePanel* stage,
			ToolbarPanel* toolbar);

		virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& data);

	private:
		ee::LibraryPanel* m_library;
		StagePanel* m_stage;
		ToolbarPanel* m_toolbar;

	}; // DropTarget 

private:
	StagePanel* m_stage;

//	wxTextCtrl* m_name;
	wxCheckBox* m_loop;
	wxCheckBox* m_local_mode_draw;

	std::vector<ee::SliderCtrl*> m_sliders;
	std::vector<std::vector<ee::SliderCtrl*> > m_mode_sliders;

	wxChoice* m_mode_choice;

	wxCheckBox* m_rotation_is_dir_ctrl;

	wxSizer* m_main_sizer;

	wxSizer* m_comp_sizer;
	std::vector<ComponentPanel*> m_children;

	friend class FileIO;

}; // ToolbarPanel

}

#endif // _EASYPARTICLE2D_TOOLBAR_PANEL_H_