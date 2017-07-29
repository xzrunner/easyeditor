#ifndef _EASYANIM_KEYS_FRAME_LERP_DIALOG_H_
#define _EASYANIM_KEYS_FRAME_LERP_DIALOG_H_

#include <sprite2/AnimLerp.h>

#include <wx/dialog.h>
#include <wx/choice.h>

#include <vector>

class wxCheckBox;

namespace s2 { class ILerp; }

namespace eanim
{

class KeyFrame;

class FrameLerpDialog : public wxDialog
{
public:
	FrameLerpDialog(wxWindow* parent, KeyFrame* frame);

	void Store();

private:
	void InitLayout();

	wxSizer* InitCircleLayout();
	wxSizer* InitSpiralLayout();
	wxSizer* InitWiggleLayout();
	wxSizer* InitEaseLayout();

	static s2::ILerp* QueryLerp(const std::vector<std::pair<s2::AnimLerp::SprData, s2::ILerp*> >& lerps,
		int data_type, int lerp_type);

private:
	KeyFrame* m_frame;

	wxCheckBox* m_pos_circle;
	wxSpinCtrl* m_circle_scale;	

	wxCheckBox* m_pos_spiral;
	wxSpinCtrl *m_spiral_angle_begin, *m_spiral_angle_end;
	wxSpinCtrl* m_spiral_scale;

	wxCheckBox* m_pos_wiggle;
	wxSpinCtrl *m_wiggle_freq, *m_wiggle_amp;

	wxChoice *m_ease_pos, *m_ease_scale, *m_ease_rotate;

}; // FrameLerpDialog

}

#endif // _EASYANIM_KEYS_FRAME_LERP_DIALOG_H_