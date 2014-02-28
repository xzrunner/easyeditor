#ifndef EPARTICLE_COCO_TOOLBAR_PANEL_H
#define EPARTICLE_COCO_TOOLBAR_PANEL_H

#include <drag2d.h>

namespace eparticle
{
namespace coco
{
	struct ParticleChild;

	class ToolbarPanel : public d2d::ToolbarPanel
	{
	public:
		ToolbarPanel(wxWindow* parent);

		void add(const d2d::ParticleFileAdapter::Child& child);

		void initParticle();

	protected:
		virtual wxSizer* initLayout();

	private:
		void clear();

		void onAddChild(wxCommandEvent& event);
		void onDelChild(wxCommandEvent& event);

		void onSetCount(wxSpinEvent& event);
		void onSetEmissionTime(wxSpinEvent& event);
		void onSetLife(wxSpinEvent& event);
		void onSetHori(wxSpinEvent& event);
		void onSetVert(wxSpinEvent& event);
		void onSetSpeed(wxSpinEvent& event);
		void onSetGravity(wxSpinEvent& event);
		void onSetInertia(wxSpinEvent& event);
		void onSetFadeoutTime(wxSpinEvent& event);
		void onSetBounce(wxCommandEvent& event);
		void onSetAdditiveBlend(wxCommandEvent& event);
		void onSetStartRadius(wxSpinEvent& event);

	private:
		static const float COUNT;
		static const float EMISSION_TIME;
		static const float MIN_LIFE;
		static const float MAX_LIFE;
		static const float MIN_HORI;
		static const float MAX_HORI;
		static const float MIN_VERT;
		static const float MAX_VERT;
		static const float MIN_SPD;
		static const float MAX_SPD;
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

			void onSetImage(wxCommandEvent& event);
			void onSetScale(wxSpinEvent& event);
			void onSetRotate(wxSpinEvent& event); 

		private:
			ParticleChild* m_pc;

			wxTextCtrl* m_filename;
			wxTextCtrl* m_name;
			wxSpinCtrl *m_start_scale, *m_end_scale;
			wxSpinCtrl *m_min_rotate, *m_max_rotate;
			wxSpinCtrl* m_startz;

			friend class FileIO;
			friend class ToolbarPanel;
			friend class Code;
		};

	private:
		d2d::Image* m_image;

		wxTextCtrl* m_name;
		wxTextCtrl* m_package;
		wxSpinCtrl* m_count;
		wxSpinCtrl* m_layer;
		wxSpinCtrl* m_emission_time;
		wxSpinCtrl *m_min_life, *m_max_life;
		wxSpinCtrl *m_min_hori, *m_max_hori;
		wxSpinCtrl *m_min_vert, *m_max_vert;
		wxSpinCtrl *m_min_spd, *m_max_spd;
		wxSpinCtrl* m_gravity;
		wxSpinCtrl* m_inertia;
		wxSpinCtrl* m_fadeout_time;
		wxCheckBox* m_bounce;
		wxCheckBox* m_additiveBlend;
		wxSpinCtrl* m_start_radius;
		wxCheckBox* m_orient_to_movement;
		wxCheckBox* m_orient_to_parent;

		wxSizer* m_compSizer;
		std::vector<ChildPanel*> m_children;

		friend class FileIO;
		friend class Code;

	}; // ToolbarPanel
}
}

#endif // EPARTICLE_COCO_TOOLBAR_PANEL_H