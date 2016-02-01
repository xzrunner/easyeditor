#ifndef _EASYSCALE9_RESIZECMPT_H_
#define _EASYSCALE9_RESIZECMPT_H_

#include <ee/EditCMPT.h>

namespace escale9
{

class StagePanel;
class ToolbarPanel;

class ResizeCMPT : public ee::EditCMPT
{
public:
	ResizeCMPT(wxWindow* parent, const std::string& name,
		StagePanel* stage, ToolbarPanel* toolbar);

	void setSize(int width, int height);
	float getWidth() const;
	float getHeight() const;

protected:
	virtual wxSizer* InitLayout();

private:
	void onChangeSize(wxSpinEvent& event);

private:
	StagePanel* m_stage;

	wxSpinCtrl *m_wSpin, *m_hSpin;

}; // ResizeCMPT

}

#endif / _EASYSCALE9_RESIZECMPT_H_