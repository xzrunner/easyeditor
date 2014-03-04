#ifndef EPARTICLE_COCO3D_TOOLBAR_PANEL_H
#define EPARTICLE_COCO3D_TOOLBAR_PANEL_H

#include <drag2d.h>

namespace eparticle
{
namespace coco3d
{
	class ToolbarPanel : public d2d::ToolbarPanel
	{
	public:
		ToolbarPanel(wxWindow* parent);

	protected:
		virtual wxSizer* initLayout();

	private:
		void enable(bool enable);

		void initParticle();

		void onOpenImage(wxCommandEvent& event);

		void onSetCount(wxSpinEvent& event);
		void onSetEmissionTime(wxSpinEvent& event);
		void onSetLife(wxSpinEvent& event);
		void onSetHori(wxSpinEvent& event);
		void onSetVert(wxSpinEvent& event);
		void onSetSpeed(wxSpinEvent& event);
		void onSetGravity(wxSpinEvent& event);
		void onSetFadeoutTime(wxSpinEvent& event);
		void onSetBounce(wxCommandEvent& event);
		void onSetScale(wxSpinEvent& event);
		void onSetRotate(wxSpinEvent& event);

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
		static const float FADEOUT_TIME;
		static const float START_SCALE;
		static const float END_SCALE;
		static const float MIN_ROTATE;
		static const float MAX_ROTATE;

	private:
		d2d::Image* m_image;

		wxSpinCtrl* m_count;
		wxSpinCtrl* m_emission_time;
		wxSpinCtrl *m_min_life, *m_max_life;
		wxSpinCtrl *m_min_hori, *m_max_hori;
		wxSpinCtrl *m_min_vert, *m_max_vert;
		wxSpinCtrl *m_min_spd, *m_max_spd;
		wxSpinCtrl* m_gravity;
		wxSpinCtrl* m_fadeout_time;
		wxCheckBox* m_bounce;
		wxSpinCtrl *m_start_scale, *m_end_scale;
		wxSpinCtrl *m_min_rotate, *m_max_rotate;

	}; // ToolbarPanel
}
}

#endif // EPARTICLE_COCO3D_TOOLBAR_PANEL_H