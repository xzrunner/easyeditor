#ifndef _LIBSHAPE_POLYLINES_CMPT_H_
#define _LIBSHAPE_POLYLINES_CMPT_H_



namespace libshape
{

class EditPolylinesCMPT : public ee::EditCMPT
{
public:
	EditPolylinesCMPT(wxWindow* parent, const wxString& name,
		ee::EditPanel* editPanel, ee::MultiShapesImpl* shapesImpl);

	virtual void updateControlValue();

	float getSimplifyThreshold() const;

protected:
	virtual wxSizer* InitLayout();

private:
	wxSizer* initSimplifyPanel();
	wxSizer* initSmoothPanel();
	wxSizer* initEditPanel();

	void onSimplifyThresholdChanged(wxSpinEvent& event);
	void onUpdateFromSimplified(wxCommandEvent& event);

	void onMergeTwoChain(wxCommandEvent& event);
	void onTranslate(wxCommandEvent& event);

private:
	ee::MultiShapesImpl* m_shapesImpl;

	wxSpinCtrl* m_simplifySpin;

	wxButton* m_btnMerge;

}; // EditPolylinesCMPT

}

#endif // _LIBSHAPE_POLYLINES_CMPT_H_