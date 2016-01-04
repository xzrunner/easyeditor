#ifndef _EASYPARTICLE3D_TOOLBAR_PANEL_H_
#define _EASYPARTICLE3D_TOOLBAR_PANEL_H_

#include "LoadAdapter.h"

#include <drag2d.h>

namespace eparticle3d
{

class StagePanel;
class ComponentPanel;

class ToolbarPanel : public d2d::ToolbarPanel, public d2d::UICallback, public d2d::Observer
{
public:
	ToolbarPanel(wxWindow* parent, d2d::LibraryPanel* library,
		StagePanel* stage);

	//
	// UICallback interface
	//
	virtual void SetValue(int key, const d2d::UICallback::Data& data);
	virtual void GetValue(int key, d2d::UICallback::Data& data);

	void Load(const Json::Value& val, int version);
	void Store(Json::Value& val) const;

	void Add(const LoadAdapter::Component& comp);

	void InitParticle();

	void OnDelChild(ComponentPanel* child);

protected:
	virtual wxSizer* initLayout();

	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	wxSizer* CreateMainLayout();
	wxSizer* CreateComponentLayout();

	void Clear();

	void OnAddChild(wxCommandEvent& event, d2d::ISymbol* symbol);
	void OnDelAllChild(wxCommandEvent& event);

	void OnSetLoop(wxCommandEvent& event);
	void OnSetLocalModeDraw(wxCommandEvent& event);
	void OnSet(wxCommandEvent& event);
	void OnSetHori(wxSpinEvent& event);
	void OnSetVert(wxSpinEvent& event);
	void OnSetGround(wxCommandEvent& event);
	void OnSetOrientToMovement(wxCommandEvent& event);

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

	d2d::Image* m_image;

	std::vector<d2d::ISliderCtrl*> m_sliders;

	// move to property
//	wxTextCtrl* m_name;
// 	wxCheckBox* m_loop;
// 	wxCheckBox* m_local_mode_draw;

	wxSpinCtrl *m_min_hori, *m_max_hori;
	wxSpinCtrl *m_min_vert, *m_max_vert;
	wxChoice* m_ground;
	wxCheckBox* m_orient_to_movement;
//	wxCheckBox* m_orient_to_parent;

	wxSizer* m_comp_sizer;
	std::vector<ComponentPanel*> m_children;

	friend class FileIO;

}; // ToolbarPanel

}

#endif // _EASYPARTICLE3D_TOOLBAR_PANEL_H_