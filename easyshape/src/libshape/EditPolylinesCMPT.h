#ifndef _EASYSHAPE_POLYLINES_CMPT_H_
#define _EASYSHAPE_POLYLINES_CMPT_H_



namespace eshape
{

class EditPolylinesCMPT : public ee::EditCMPT
{
public:
	EditPolylinesCMPT(wxWindow* parent, const std::string& name,
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

#endif // _EASYSHAPE_POLYLINES_CMPT_H_