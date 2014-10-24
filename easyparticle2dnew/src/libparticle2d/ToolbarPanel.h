#ifndef _EASYPARTICLE2D_TOOLBAR_PANEL_H_
#define _EASYPARTICLE2D_TOOLBAR_PANEL_H_

#include <drag2d.h>

namespace eparticle2d
{

class ParticleSystem;

class ToolbarPanel : public d2d::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent, d2d::LibraryPanel* library,
		d2d::EditPanel* stage, ParticleSystem* ps);
	virtual ~ToolbarPanel();

protected:
	virtual wxSizer* initLayout();

private:
	void InitPSValue();

	void OnSetCount(wxScrollEvent& event);
	void OnSetEmissionTime(wxScrollEvent& event);
	void OnSetFadeoutTime(wxScrollEvent& event);

	void OnSetLife(wxScrollEvent& event);
	void OnSetPosition(wxScrollEvent& event);
	void OnSetSpeed(wxScrollEvent& event);

private:
	class DropTarget : public wxTextDropTarget
	{
	public:
		DropTarget(d2d::LibraryPanel* library, ParticleSystem* ps);

		virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& data);

	private:
		d2d::LibraryPanel* m_library;

		ParticleSystem* m_ps;

	}; // DropTarget

private:
	ParticleSystem* m_ps;
	
	wxSlider* m_count;
	wxSlider* m_emission_time;
	wxSlider* m_fadeout_time;

	wxSlider *m_life_min, *m_life_max;
	wxSlider *m_pos_x_min, *m_pos_x_max, *m_pos_y_min, *m_pos_y_max;
	wxSlider *m_spd_x_min, *m_spd_x_max, *m_spd_y_min, *m_spd_y_max;

}; // ToolbarPanel

}

#endif // _EASYPARTICLE2D_TOOLBAR_PANEL_H_