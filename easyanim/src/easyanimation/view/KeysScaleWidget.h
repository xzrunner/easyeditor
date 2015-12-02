#ifndef _EASYANIM_KEYS_SCALE_WIDGET_H_
#define _EASYANIM_KEYS_SCALE_WIDGET_H_

#include <drag2d.h>

namespace eanim
{

class LayersMgr;

class KeysScaleWidget : public wxPanel, public d2d::Observer
{
public:
	KeysScaleWidget(wxWindow* parent, const LayersMgr& layers);

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
	int QueryGridByPos(float x) const;

private:
	const LayersMgr& m_layers;

	int m_curr_frame;

private:
	DECLARE_EVENT_TABLE()

}; // KeysScaleWidget

}

#endif // _EASYANIM_KEYS_SCALE_WIDGET_H_