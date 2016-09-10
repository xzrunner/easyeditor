#ifndef _EASYPARTICLE3D_TOOLBAR_PANEL_H_
#define _EASYPARTICLE3D_TOOLBAR_PANEL_H_

#include <ee/ToolbarPanel.h>
#include <ee/UICallback.h>
#include <ee/Observer.h>

#include <gum/P3dSymLoader.h>

#include <wx/dnd.h>

namespace ee { class LibraryPanel; class Image; class SliderCtrl; }
namespace s2 { class Symbol; }

namespace eparticle3d
{

class StagePanel;
class ComponentPanel;

class ToolbarPanel : public ee::ToolbarPanel, public ee::UICallback, public ee::Observer
{
public:
	ToolbarPanel(wxWindow* parent, ee::LibraryPanel* library,
		StagePanel* stage);

	//
	// UICallback interface
	//
	virtual void SetValue(int key, const ee::UICallback::Data& data);
	virtual void GetValue(int key, ee::UICallback::Data& data);

	void Load(const Json::Value& val, int version);
	void Store(Json::Value& val) const;

	void Add(const gum::P3dSymLoader::Component& comp, ee::LibraryPanel* library);

	void InitParticle();

	void OnDelChild(ComponentPanel* child);

	void OnSetStaticMode(bool static_mode);

protected:
	virtual wxSizer* InitLayout();

	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	wxSizer* CreateMainLayout();
	wxSizer* CreateComponentLayout();

	void Clear();

	void OnAddChild(wxCommandEvent& event, s2::Symbol* sym);
	void OnDelAllChild(wxCommandEvent& event);

	void OnSetStaticMode(wxCommandEvent& event);

	void OnSetLoop(wxCommandEvent& event);
	void OnSetLocalModeDraw(wxCommandEvent& event);
	void OnSet(wxCommandEvent& event);
	void OnSetHori(wxSpinEvent& event);
	void OnSetVert(wxSpinEvent& event);
	void OnSetGround(wxCommandEvent& event);
	void OnSetOrientToMovement(wxCommandEvent& event);
	void OnSetBlend(wxCommandEvent& event);

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

	ee::Image* m_image;

	std::vector<ee::SliderCtrl*> m_sliders;

	wxCheckBox* m_static_mode;

	wxPanel *m_count_ctrl, *m_time_ctrl;

	// move to property
//	wxTextCtrl* m_name;
// 	wxCheckBox* m_loop;
// 	wxCheckBox* m_local_mode_draw;

	wxSpinCtrl *m_min_hori, *m_max_hori;
	wxSpinCtrl *m_min_vert, *m_max_vert;
	wxChoice* m_ground;
	wxCheckBox* m_orient_to_movement;
//	wxCheckBox* m_orient_to_parent;
	wxChoice* m_blend;

	wxSizer* m_comp_sizer;
	std::vector<ComponentPanel*> m_children;

	friend class FileIO;

}; // ToolbarPanel

}

#endif // _EASYPARTICLE3D_TOOLBAR_PANEL_H_