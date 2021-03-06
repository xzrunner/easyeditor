#ifndef _EASYANIM_KEYS_CONTENT_WIDGET_H_
#define _EASYANIM_KEYS_CONTENT_WIDGET_H_

#include "config.h"
#include "KeysContentEdit.h"

#include <ee/KeysState.h>

#include <wx/panel.h>

class wxBufferedPaintDC;

namespace eanim
{

class KeysPanel;
class LayersMgr;
class Layer;
class KeyFrame;

class KeysContentWidget : public wxPanel, public ee::Observer
{
public:
	KeysContentWidget(wxWindow* parent);

	void OnSize(wxSizeEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnEraseBackground(wxEraseEvent& event);
	void OnMouse(wxMouseEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud) override;

private:
	bool IsPosOnKeyFrame() const;

	void DrawBackground(wxBufferedPaintDC& dc);
	void DrawLayersDataBg(wxBufferedPaintDC& dc);
	void DrawLayersDataFlag(wxBufferedPaintDC& dc);
	void DrawCurrPosFlag(wxBufferedPaintDC& dc);
	void DrawSelected(wxBufferedPaintDC& dc);

	void MousePopupMenu(int x, int y);

	void OnCreateClassicTween(wxCommandEvent& event);
	void OnDeleteClassicTween(wxCommandEvent& event);
	void OnInsertFrame(wxCommandEvent& event);
	void OnDeleteFrame(wxCommandEvent& event);
	void OnInsertKeyFrame(wxCommandEvent& event);
	void OnDeleteKeyFrame(wxCommandEvent& event);

	void OnUpdateCreateClassicTween(wxUpdateUIEvent& event);
	void OnUpdateDeleteClassicTween(wxUpdateUIEvent& event);
	void OnUpdateInsertFrame(wxUpdateUIEvent& event);
	void OnUpdateDeleteFrame(wxUpdateUIEvent& event);
	void OnUpdateInsertKeyFrame(wxUpdateUIEvent& event);
	void OnUpdateDeleteKeyFrame(wxUpdateUIEvent& event);

	void OnInsertFrame(wxKeyEvent& event);
	void OnDeleteFrame(wxKeyEvent& event);

private:
	void OnInsertFrame();
	void OnDeleteFrame();

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
	KeysContentEdit m_editop;

	ee::KeysState m_keys_state;

	int m_layer_idx, m_frame_idx;
	int m_valid_frame_idx;
	int m_col_min, m_col_max;

	Layer* m_layer;
	KeyFrame* m_frame;

	DECLARE_EVENT_TABLE()

}; // KeysContentWidget

}

#endif // _EASYANIM_KEYS_CONTENT_WIDGET_H_