#ifndef _EASYSHAPE_POLYLINES_CMPT_H_
#define _EASYSHAPE_POLYLINES_CMPT_H_

#include <ee/EditCMPT.h>

namespace ee { class EditPanel; class MultiShapesImpl; }

namespace eshape
{

class EditPolylinesCMPT : public ee::EditCMPT
{
public:
	EditPolylinesCMPT(wxWindow* parent, const std::string& name,
		ee::EditPanel* editPanel, ee::MultiShapesImpl* shapes_impl);

	virtual void UpdateControlValue();

	float GetSimplifyThreshold() const;

protected:
	virtual wxSizer* InitLayout();

private:
	wxSizer* InitSimplifyPanel();
	wxSizer* InitSmoothPanel();
	wxSizer* InitEditPanel();

	void OnSimplifyThresholdChanged(wxSpinEvent& event);
	void OnUpdateFromSimplified(wxCommandEvent& event);

	void OnMergeTwoChain(wxCommandEvent& event);
	void OnTranslate(wxCommandEvent& event);

private:
	ee::MultiShapesImpl* m_shapes_impl;

	wxSpinCtrl* m_simplify_spin;

	wxButton* m_btn_merge;

}; // EditPolylinesCMPT

}

#endif // _EASYSHAPE_POLYLINES_CMPT_H_