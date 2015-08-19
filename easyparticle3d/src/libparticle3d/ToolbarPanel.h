#ifndef _EASYPARTICLE3D_TOOLBAR_PANEL_H_
#define _EASYPARTICLE3D_TOOLBAR_PANEL_H_

#include "FileAdapter.h"

#include <drag2d.h>

// struct ps_color4f;
// struct particle_symbol;

namespace eparticle3d
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

	void Load(const Json::Value& val, int version);
	void Store(Json::Value& val) const;

	void Add(const FileAdapter::Component& comp);

	void InitParticle();

	void OnDelChild(ComponentPanel* child);

protected:
	virtual wxSizer* initLayout();

private:
	void clear();

	void OnAddChild(wxCommandEvent& event, d2d::ISymbol* symbol);
	void OnDelAllChild(wxCommandEvent& event);

	void OnSetHori(wxSpinEvent& event);
	void OnSetVert(wxSpinEvent& event);
	void OnSetInertia(wxSpinEvent& event);
	void OnSetBounce(wxCommandEvent& event);
	void OnSetAdditiveBlend(wxCommandEvent& event);
	void OnSetOrientToMovement(wxCommandEvent& event);
	void OnSetRadius3D(wxCommandEvent& event);

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

	wxTextCtrl* m_name;
	wxTextCtrl* m_package;
	wxSpinCtrl* m_layer;
	wxSpinCtrl *m_min_hori, *m_max_hori;
	wxSpinCtrl *m_min_vert, *m_max_vert;
	wxSpinCtrl* m_inertia;
	wxCheckBox* m_bounce;
	wxCheckBox* m_additiveBlend;
	wxCheckBox* m_orient_to_movement;
	wxCheckBox* m_orient_to_parent;
	wxCheckBox* m_radius_3d;

	wxSizer* m_compSizer;
	std::vector<ComponentPanel*> m_children;

	friend class FileIO;

}; // ToolbarPanel

}

#endif // _EASYPARTICLE3D_TOOLBAR_PANEL_H_