#ifndef _LIBSHAPE_POLYLINES_CMPT_H_
#define _LIBSHAPE_POLYLINES_CMPT_H_

#include <drag2d.h>

namespace libshape
{

class EditPolylinesCMPT : public d2d::AbstractEditCMPT
{
public:
	EditPolylinesCMPT(wxWindow* parent, const wxString& name,
		d2d::EditPanel* editPanel, d2d::MultiShapesImpl* shapesImpl,
		d2d::PropertySettingPanel* propertyPanel);

	virtual void updateControlValue();

	float getSimplifyThreshold() const;

protected:
	virtual wxSizer* initLayout();

private:
	wxSizer* initSimplifyPanel();
	wxSizer* initSmoothPanel();
	wxSizer* initEditPanel();

	void onSimplifyThresholdChanged(wxSpinEvent& event);
	void onUpdateFromSimplified(wxCommandEvent& event);

	void onMergeTwoChain(wxCommandEvent& event);
	void onTranslate(wxCommandEvent& event);

private:
	d2d::MultiShapesImpl* m_shapesImpl;

	wxSpinCtrl* m_simplifySpin;

	wxButton* m_btnMerge;

}; // EditPolylinesCMPT

}

#endif // _LIBSHAPE_POLYLINES_CMPT_H_