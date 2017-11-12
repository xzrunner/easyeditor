#ifndef _EASYANIM_LAYERS_CONTENT_WIDGET_H_
#define _EASYANIM_LAYERS_CONTENT_WIDGET_H_

#include <ee/Observer.h>

#include <wx/panel.h>

namespace eanim
{

class LayersContentWidget : public wxPanel, public ee::Observer
{
public:
	LayersContentWidget(wxWindow* parent);

	//
	//	interface wxPanel
	//
	//// fix me: after update wx to 3.1, there are no these api in wxPanel
	//virtual wxCoord OnGetRowHeight(size_t row) const override;
	//virtual int GetNonOrientationTargetSize() const override;
	//virtual wxOrientation GetOrientation() const override;

	void OnSize(wxSizeEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnEraseBackground(wxEraseEvent& event);
	void OnMouse(wxMouseEvent& event);

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud) override;

private:
	int m_drag_flag_line;

	int m_curr_layer;

	DECLARE_EVENT_TABLE()

}; // LayersContentWidget

}

#endif // _EASYANIM_LAYERS_CONTENT_WIDGET_H_