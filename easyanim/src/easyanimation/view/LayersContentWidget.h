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
	virtual wxCoord OnGetRowHeight(size_t row) const;
	virtual int GetNonOrientationTargetSize() const;
	virtual wxOrientation GetOrientation() const;

	void OnSize(wxSizeEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnEraseBackground(wxEraseEvent& event);
	void OnMouse(wxMouseEvent& event);

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	int m_drag_flag_line;

	int m_curr_layer;

	DECLARE_EVENT_TABLE()

}; // LayersContentWidget

}

#endif // _EASYANIM_LAYERS_CONTENT_WIDGET_H_