#ifndef _EASYPARTICLE3D_TOOLBAR_PANEL_H_
#define _EASYPARTICLE3D_TOOLBAR_PANEL_H_

#include "FileAdapter.h"

#include <drag2d.h>

namespace eparticle3d
{

class StagePanel;
struct ParticleChild;

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

	void add(const FileAdapter::Child& child);

	void initParticle();

protected:
	virtual wxSizer* initLayout();

private:
	void clear();

	void onStoreRecord(wxCommandEvent& event);

	void onAddChild(wxCommandEvent& event);
	void onDelChild(wxCommandEvent& event);

	void onSetHori(wxSpinEvent& event);
	void onSetVert(wxSpinEvent& event);
	void onSetInertia(wxSpinEvent& event);
	void onSetBounce(wxCommandEvent& event);
	void onSetAdditiveBlend(wxCommandEvent& event);
	void OnSetOrientToMovement(wxCommandEvent& event);

private:
	static const float COUNT;
	static const float EMISSION_TIME;
	static const float LIFE_CENTER, LIFE_OFFSET;
	static const float MIN_HORI, MAX_HORI;
	static const float MIN_VERT, MAX_VERT;
	static const float SPEED_CENTER, SPEED_OFFSET;
	static const float ANGULAR_SPEED_CENTER, ANGULAR_SPEED_OFFSET;
	static const float GRAVITY;
	static const float INERTIA;
	static const float FADEOUT_TIME;
	static const float START_RADIUS;
	static const float START_SCALE;
	static const float END_SCALE;
	static const float MIN_ROTATE;
	static const float MAX_ROTATE;

private:
	class ChildPanel : public wxPanel
	{
	public:
		ChildPanel(wxWindow* parent, ParticleChild* pc);

	private:
		void initLayout();

		void onSetScale(wxScrollEvent& event);
		void onSetRotate(wxScrollEvent& event); 

		void OnBindPS(wxCommandEvent& event);

	private:
		ParticleChild* m_pc;

		wxTextCtrl* m_name;
		wxSlider *m_start_scale, *m_end_scale;
		wxSlider *m_min_rotate, *m_max_rotate;
		wxSpinCtrl* m_startz;

		friend class ToolbarPanel;
		friend class Code;
		friend class FileIO;
	};

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

	wxSizer* m_compSizer;
	std::vector<ChildPanel*> m_children;

	friend class Code;
	friend class FileIO;

}; // ToolbarPanel

}

#endif // _EASYPARTICLE3D_TOOLBAR_PANEL_H_