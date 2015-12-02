#ifndef _EASYANIM_LAYERS_CONTENT_WIDGET_H_
#define _EASYANIM_LAYERS_CONTENT_WIDGET_H_

#include <drag2d.h>

namespace eanim
{

class LayersMgr;

class LayersContentWidget : public wxPanel, public d2d::Observer
{
public:
	LayersContentWidget(wxWindow* parent, LayersMgr& layers);

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
	LayersMgr& m_layers;

	int m_drag_flag_line;

	int m_curr_layer;

	DECLARE_EVENT_TABLE()

}; // LayersContentWidget

}

#endif // _EASYANIM_LAYERS_CONTENT_WIDGET_H_