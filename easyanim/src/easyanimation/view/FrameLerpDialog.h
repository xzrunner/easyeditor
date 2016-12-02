#ifndef _EASYANIM_KEYS_FRAME_LERP_DIALOG_H_
#define _EASYANIM_KEYS_FRAME_LERP_DIALOG_H_

#include <wx/dialog.h>

class wxCheckBox;

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

private:
	KeyFrame* m_frame;

	wxCheckBox* m_pos_spiral;
	wxSpinCtrl *m_spiral_angle_begin, *m_spiral_angle_end;

}; // FrameLerpDialog

}

#endif // _EASYANIM_KEYS_FRAME_LERP_DIALOG_H_