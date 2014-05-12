#pragma once
#include <wx/wx.h>
#include "Utility.h"

class wxBufferedPaintDC;

namespace eanim
{
	class KeysPanel;
	class LayersMgr;
	class KeyFrame;

	class KeysContentWidget : public wxPanel
	{
	public:
		KeysContentWidget(KeysPanel* parent, int id, LayersMgr* layersMgr);

		void onSize(wxSizeEvent& event);
		void onPaint(wxPaintEvent& event);
		void onEraseBackground(wxEraseEvent& event);
		void onMouse(wxMouseEvent& event);

	private:
		KeyFrame* queryKeyFrameByPos() const;

		void drawBackground(wxBufferedPaintDC& dc);
		void drawLayersDataBg(wxBufferedPaintDC& dc);
		void drawLayersDataFlag(wxBufferedPaintDC& dc);
		void drawCurrPosFlag(wxBufferedPaintDC& dc);
		void drawSelected(wxBufferedPaintDC& dc);

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

	private:
		enum
		{
			Menu_CreateClassicTween,
			Menu_DeleteClassicTween,
			Menu_InsertFrame,
			Menu_DeleteFrame,
			Menu_InsertKeyFrame,
			Menu_DeleteKeyFrame
		};

		static LanguageEntry entries[];

		static const int KEY_FRAME_CIRCLE_Y_OFFSET = FRAME_GRID_WIDTH * 0.5f + 1;
		static const int KEY_FRAME_CIRCLE_RADIUS = FRAME_GRID_WIDTH * 0.4f;
		static const int KEY_FRAME_RECT_Y_OFFSET = KEY_FRAME_CIRCLE_Y_OFFSET + 1;
		static const int FRAME_END_RECT_WIDTH = KEY_FRAME_CIRCLE_RADIUS * 2;
		static const int FRAME_END_RECT_HEIGHT = KEY_FRAME_CIRCLE_RADIUS * 3;

	private:
		KeysPanel* m_parent;
		LayersMgr* m_layersMgr;

		DECLARE_EVENT_TABLE()

	}; // KeysContentWidget
}

