#ifndef _EASYANIM_KEYS_CONTENT_WIDGET_H_
#define _EASYANIM_KEYS_CONTENT_WIDGET_H_

#include <drag2d.h>

#include "Utility.h"
#include "KeysContentEdit.h"

class wxBufferedPaintDC;

namespace eanim
{

class KeysPanel;
class LayersMgr;
class KeyFrame;
class Controller;

class KeysContentWidget : public wxPanel, public d2d::Observer
{
public:
	KeysContentWidget(wxWindow* parent, Controller* ctrl, const LayersMgr& layers);

	//
	//	interface Observer
	//
	virtual void Notify(int sj_id, void* ud);

	void onSize(wxSizeEvent& event);
	void onPaint(wxPaintEvent& event);
	void onEraseBackground(wxEraseEvent& event);
	void onMouse(wxMouseEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);

private:
	KeyFrame* queryKeyFrameByPos() const;
	bool isPosOnKeyFrame() const;

	void drawBackground(wxBufferedPaintDC& dc);
	void drawLayersDataBg(wxBufferedPaintDC& dc);
	void drawLayersDataFlag(wxBufferedPaintDC& dc);
	void drawCurrPosFlag(wxBufferedPaintDC& dc);
	void drawSelected(wxBufferedPaintDC& dc);

	void MousePopupMenu(int x, int y);

	void onCreateClassicTween(wxCommandEvent& event);
	void onDeleteClassicTween(wxCommandEvent& event);
	void onInsertFrame(wxCommandEvent& event);
	void onDeleteFrame(wxCommandEvent& event);
	void onInsertKeyFrame(wxCommandEvent& event);
	void onDeleteKeyFrame(wxCommandEvent& event);

	void onUpdateCreateClassicTween(wxUpdateUIEvent& event);
	void onUpdateDeleteClassicTween(wxUpdateUIEvent& event);
	void onUpdateInsertFrame(wxUpdateUIEvent& event);
	void onUpdateDeleteFrame(wxUpdateUIEvent& event);
	void onUpdateInsertKeyFrame(wxUpdateUIEvent& event);
	void onUpdateDeleteKeyFrame(wxUpdateUIEvent& event);

	void onInsertFrame(wxKeyEvent& event);
	void onDeleteFrame(wxKeyEvent& event);

private:
	void onInsertFrame();
	void onDeleteFrame();

private:
	enum
	{
		Menu_CreateClassicTween,
		Menu_DeleteClassicTween,
		Menu_InsertFrame,
		Menu_DeleteFrame,
		Menu_InsertKeyFrame,
		Menu_DeleteKeyFrame,

		Hot_InsertFrame,
		Hot_DeleteFrame
	};

	static LanguageEntry entries[];

	static const int KEY_FRAME_CIRCLE_Y_OFFSET = FRAME_GRID_WIDTH * 0.5f + 1;
	static const int KEY_FRAME_CIRCLE_RADIUS = FRAME_GRID_WIDTH * 0.4f;
	static const int KEY_FRAME_RECT_Y_OFFSET = KEY_FRAME_CIRCLE_Y_OFFSET + 1;
	static const int FRAME_END_RECT_WIDTH = KEY_FRAME_CIRCLE_RADIUS * 2;
	static const int FRAME_END_RECT_HEIGHT = KEY_FRAME_CIRCLE_RADIUS * 3;

private:
	const LayersMgr& m_layers;

	KeysContentEdit m_editop;

	d2d::KeysState m_keys_state;

	int m_curr_layer, m_curr_frame;

	DECLARE_EVENT_TABLE()

}; // KeysContentWidget

}

#endif // _EASYANIM_KEYS_CONTENT_WIDGET_H_