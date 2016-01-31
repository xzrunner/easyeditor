#ifndef _EASYANIM_KEYS_SCALE_WIDGET_H_
#define _EASYANIM_KEYS_SCALE_WIDGET_H_



namespace eanim
{

class Layer;

class KeysScaleWidget : public wxPanel, public ee::Observer
{
public:
	KeysScaleWidget(wxWindow* parent);

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
	int m_frame_idx;

	Layer* m_layer;

private:
	DECLARE_EVENT_TABLE()

}; // KeysScaleWidget

}

#endif // _EASYANIM_KEYS_SCALE_WIDGET_H_