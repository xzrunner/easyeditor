#include "KeysContentWidget.h"
#include "KeysPanel.h"

#include "dataset/KeyFrame.h"
#include "dataset/Layer.h"
#include "dataset/DataMgr.h"
#include "dataset/data_utility.h"
#include "view/StagePanel.h"
#include "view/ViewMgr.h"
#include "view/view_utility.h"
#include "message/messages.h"

#include <wx/dcbuffer.h>

namespace eanim
{

BEGIN_EVENT_TABLE(KeysContentWidget, wxPanel)
	EVT_PAINT(KeysContentWidget::OnPaint)
	EVT_ERASE_BACKGROUND(KeysContentWidget::OnEraseBackground)
	EVT_SIZE(KeysContentWidget::OnSize)
	EVT_MOUSE_EVENTS(KeysContentWidget::OnMouse)
	EVT_KEY_DOWN(KeysContentWidget::OnKeyDown)
	EVT_KEY_UP(KeysContentWidget::OnKeyUp)

	EVT_MENU(Menu_CreateClassicTween, KeysContentWidget::OnCreateClassicTween)
	EVT_MENU(Menu_DeleteClassicTween, KeysContentWidget::OnDeleteClassicTween)
	EVT_MENU(Menu_InsertFrame, KeysContentWidget::OnInsertFrame)
	EVT_MENU(Menu_DeleteFrame, KeysContentWidget::OnDeleteFrame)
	EVT_MENU(Menu_InsertKeyFrame, KeysContentWidget::OnInsertKeyFrame)
	EVT_MENU(Menu_DeleteKeyFrame, KeysContentWidget::OnDeleteKeyFrame)

	EVT_UPDATE_UI(Menu_CreateClassicTween, KeysContentWidget::OnUpdateCreateClassicTween)
	EVT_UPDATE_UI(Menu_DeleteClassicTween, KeysContentWidget::OnUpdateDeleteClassicTween)
	EVT_UPDATE_UI(Menu_InsertFrame, KeysContentWidget::OnUpdateInsertFrame)
	EVT_UPDATE_UI(Menu_DeleteFrame, KeysContentWidget::OnUpdateDeleteFrame)
	EVT_UPDATE_UI(Menu_InsertKeyFrame, KeysContentWidget::OnUpdateInsertKeyFrame)
	EVT_UPDATE_UI(Menu_DeleteKeyFrame, KeysContentWidget::OnUpdateDeleteKeyFrame)

	EVT_HOTKEY(Hot_InsertFrame, KeysContentWidget::OnInsertFrame)
	EVT_HOTKEY(Hot_DeleteFrame, KeysContentWidget::OnDeleteFrame)
END_EVENT_TABLE()

LanguageEntry KeysContentWidget::entries[] =
{
	{ "创建传统补间", "Create Classic Tween" },
	{ "删除传统补间", "Delete Classic Tween" },
	{ "插入帧", "Insert Frame" },
	{ "删除帧", "Delete Frame" },
	{ "插入关键帧", "Insert Key Frame" },
	{ "删除关键帧", "Delete Key Frame" }
};

KeysContentWidget::KeysContentWidget(wxWindow* parent)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE)
{
	m_curr_layer = m_curr_frame = -1;

//  	RegisterHotKey(Hot_InsertFrame, 0, VK_ADD);
//  	RegisterHotKey(Hot_DeleteFrame, 0, VK_SUBTRACT);

	RegisterHotKey(Hot_InsertFrame, 0, VK_MULTIPLY);
	RegisterHotKey(Hot_DeleteFrame, 0, VK_DIVIDE);

	RegistSubject(SetCurrFrameSJ::Instance());
}

void KeysContentWidget::OnSize(wxSizeEvent& event)
{
	Refresh(true);
}

void KeysContentWidget::OnPaint(wxPaintEvent& event)
{
	wxBufferedPaintDC dc(this);

	DrawBackground(dc);
	DrawLayersDataBg(dc);
	DrawSelected(dc);
	DrawLayersDataFlag(dc);
	DrawCurrPosFlag(dc);
}

void KeysContentWidget::OnEraseBackground(wxEraseEvent& event)
{
}

void KeysContentWidget::OnMouse(wxMouseEvent& event)
{
	int row = event.GetY() / FRAME_GRID_HEIGHT,
		col = event.GetX() / FRAME_GRID_WIDTH;

	if (event.RightDown()) {
		MousePopupMenu(event.GetX(), event.GetY());
	} else if (event.LeftDown()) {
		m_editop.OnMouseLeftDown(row, col);
	} else if (event.LeftUp()) {
		m_editop.OnMouseLeftUp(row, col);
	} else if (event.Dragging()) {
		m_editop.OnMouseDragging(row, col);
	} else if (event.Moving()) {
		SetFocus();
	}
}

void KeysContentWidget::OnKeyDown(wxKeyEvent& event)
{
	int key_code = event.GetKeyCode();
	m_keys_state.OnKeyDown(key_code);
	if (m_keys_state.GetKeyState(WXK_CONTROL) && (key_code == 'c' || key_code == 'C')) {
		m_editop.CopySelection();
	} else if (m_keys_state.GetKeyState(WXK_CONTROL) && (key_code == 'v' || key_code == 'V')) {
		m_editop.PasteSelection();
	} else if (key_code == WXK_DELETE) {
		m_editop.DeleteSelection();
	} else if (m_keys_state.GetKeyState(WXK_CONTROL) && (key_code == 'z' || key_code == 'Z')) {
		d2d::EditUndoSJ::Instance()->Undo();
	} else if (m_keys_state.GetKeyState(WXK_CONTROL) && (key_code == 'y' || key_code == 'Y')) {
		d2d::EditRedoSJ::Instance()->Redo();
	}
}

void KeysContentWidget::OnKeyUp(wxKeyEvent& event)
{
	int key_code = event.GetKeyCode();
	m_keys_state.OnKeyUp(key_code);
}

void KeysContentWidget::OnNotify(int sj_id, void* ud)
{
	if (sj_id == MSG_SET_CURR_FRAME) {
		SetCurrFrameSJ::CurrFrame* cf = (SetCurrFrameSJ::CurrFrame*)ud;
		if (cf->layer != -1 && cf->layer != m_curr_layer ||
			cf->frame != -1 && cf->frame != m_curr_frame) {
				m_curr_layer = cf->layer;
				m_curr_frame = cf->frame;
				Refresh();
		}
	}
}

KeyFrame* KeysContentWidget::QueryKeyFrameByPos() const
{
	int col;
	Layer* layer = get_curr_layer(col);
	if (!layer) {
		return NULL;
	} else {
		return layer->GetCurrKeyFrame(col + 1);
	}
}

bool KeysContentWidget::IsPosOnKeyFrame() const
{
	int col;
	Layer* layer = get_curr_layer(col);
	if (!layer) {
		return false;
	} else {
		const std::map<int, KeyFrame*>& frames = layer->GetAllFrames();
		return frames.find(col + 1) != frames.end();
	}
}

void KeysContentWidget::DrawBackground(wxBufferedPaintDC& dc)
{
	// background
	dc.SetPen(wxPen(LIGHT_GRAY));
	dc.SetBrush(wxBrush(LIGHT_GRAY));
	dc.DrawRectangle(GetSize());

	const size_t size = DataMgr::Instance()->GetLayers().Size();
	const float width = FRAME_GRID_WIDTH * MAX_FRAME_COUNT,
		height = FRAME_GRID_HEIGHT * size;

	// white grids
	dc.SetPen(*wxWHITE_PEN);
	dc.SetBrush(*wxWHITE_BRUSH);
 	dc.DrawRectangle(0, 0, width, height);

	// 5 times gray grids
	dc.SetPen(wxPen(LIGHT_GRAY));
	dc.SetBrush(wxBrush(LIGHT_GRAY));
	for (size_t i = 1; i <= MAX_FRAME_COUNT; ++i)
	{
		if (i % 5 == 0)
		{
			const float x = FRAME_GRID_WIDTH * (i - 1);
			dc.DrawRectangle(x, 0, FRAME_GRID_WIDTH, height);
		}
	}

	// grid lines
	dc.SetPen(wxPen(MEDIUM_GRAY));
	for (size_t i = 0; i <= size; ++i)
	{
		const float y = FRAME_GRID_HEIGHT * i;
		dc.DrawLine(0, y, width, y);
	}
	for (size_t i = 0; i <= MAX_FRAME_COUNT; ++i)
	{
		const float x = FRAME_GRID_WIDTH * i;
		dc.DrawLine(x, 0, x, height);
	}
}

void KeysContentWidget::DrawLayersDataBg(wxBufferedPaintDC& dc)
{
	for (size_t i = 0, n = DataMgr::Instance()->GetLayers().Size(); i < n; ++i)
	{
		size_t storeIndex = n - i - 1;
		const std::map<int, KeyFrame*>& frames = DataMgr::Instance()->GetLayers().GetLayer(storeIndex)->GetAllFrames();
		std::map<int, KeyFrame*>::const_iterator itr;
		// during
		for (itr = frames.begin(); itr != frames.end(); ++itr)
		{
			std::map<int, KeyFrame*>::const_iterator next = itr;
			++next;
			float x = FRAME_GRID_WIDTH * (itr->first - 1),
				y = FRAME_GRID_HEIGHT * i;
			float width = FRAME_GRID_WIDTH + 1,
				height = FRAME_GRID_HEIGHT + 1;
			if (next != frames.end())
				width = std::max(FRAME_GRID_WIDTH * (next->first - itr->first), FRAME_GRID_WIDTH) + 1;
			dc.SetPen(*wxBLACK_PEN);
			//			dc.SetBrush(*wxWHITE_BRUSH);
			dc.SetBrush(wxColour(255, 255, 255, 128));
			dc.DrawRectangle(x, y, width, height);
		}
		// Classic Tween
		for (itr = frames.begin(); itr != frames.end(); ++itr)
		{
			if (itr->second->HasClassicTween())
			{
				std::map<int, KeyFrame*>::const_iterator next = itr;
				++next;
				if (next != frames.end())
				{
					float x = FRAME_GRID_WIDTH * (itr->first - 1.0f),
						y = FRAME_GRID_HEIGHT * i;
					float width = FRAME_GRID_WIDTH * (next->first - itr->first) + 1,
						height = FRAME_GRID_HEIGHT + 1;

					dc.SetPen(*wxBLACK_PEN);
					dc.SetBrush(wxBrush(CLASSIC_TWEEN_COLOR));
					dc.DrawRectangle(x, y, width, height);
				}
			}
		}
	}
}

void KeysContentWidget::DrawLayersDataFlag(wxBufferedPaintDC& dc)
{
	for (size_t i = 0, n = DataMgr::Instance()->GetLayers().Size(); i < n; ++i)
	{
		size_t storeIndex = n - i - 1;
		const std::map<int, KeyFrame*>& frames = DataMgr::Instance()->GetLayers().GetLayer(storeIndex)->GetAllFrames();
		std::map<int, KeyFrame*>::const_iterator itr;
		// key frame start (circle)
		for (itr = frames.begin(); itr != frames.end(); ++itr)
		{
			float x = FRAME_GRID_WIDTH * (itr->first - 0.5f),
				y = FRAME_GRID_HEIGHT * (i + 1) - KEY_FRAME_CIRCLE_Y_OFFSET;
			dc.SetPen(*wxBLACK_PEN);
			if (itr->second->IsEmpty()) dc.SetBrush(*wxWHITE_BRUSH);
			else dc.SetBrush(*wxBLACK_BRUSH);
			dc.DrawCircle(x, y, KEY_FRAME_CIRCLE_RADIUS);
		}
		// key frame end (rect)
		for (itr = frames.begin(); itr != frames.end(); ++itr)
		{
			if (itr == frames.begin()) continue;

			std::map<int, KeyFrame*>::const_iterator prev = itr;
			--prev;
			if (prev->first == itr->first - 1) continue;

			float x = FRAME_GRID_WIDTH * (itr->first - 1 - 0.5f) - FRAME_END_RECT_WIDTH * 0.5f,
				y = FRAME_GRID_HEIGHT * (i + 1) - KEY_FRAME_RECT_Y_OFFSET - FRAME_END_RECT_HEIGHT * 0.5f;
			dc.SetPen(*wxBLACK_PEN);
			dc.SetBrush(*wxWHITE_BRUSH);
			dc.DrawRectangle(x, y, FRAME_END_RECT_WIDTH, FRAME_END_RECT_HEIGHT);
		}
	}
}

void KeysContentWidget::DrawCurrPosFlag(wxBufferedPaintDC& dc)
{
	int frame_idx = get_curr_frame_index();
	if (frame_idx < 0) {
		return;
	}
	const float x = FRAME_GRID_WIDTH * (frame_idx + 0.5f);
	dc.SetPen(DARK_RED);
	dc.DrawLine(x, 0, x, FRAME_GRID_HEIGHT * DataMgr::Instance()->GetLayers().Size());
}

void KeysContentWidget::DrawSelected(wxBufferedPaintDC& dc)
{
	int layer_idx, frame_idx;
	get_curr_index(layer_idx, frame_idx);
	if (layer_idx != -1 && frame_idx != -1)
	{
		dc.SetPen(wxPen(DARK_BLUE));
		dc.SetBrush(wxBrush(DARK_BLUE));
		dc.DrawRectangle(
			FRAME_GRID_WIDTH * frame_idx, 
			FRAME_GRID_HEIGHT * (DataMgr::Instance()->GetLayers().Size() - 1 - layer_idx), 
			FRAME_GRID_WIDTH, 
			FRAME_GRID_HEIGHT);
	}

	int col_min, col_max;
	ViewMgr::Instance()->keys->GetSelectRegion(layer_idx, col_min, col_max);
	if (col_min != -1 && col_max != -1 && col_min != col_max)
	{
		dc.SetPen(wxPen(DARK_BLUE));
		dc.SetBrush(wxBrush(DARK_BLUE));
		dc.DrawRectangle(FRAME_GRID_WIDTH * col_min, FRAME_GRID_HEIGHT * layer_idx, FRAME_GRID_WIDTH * (col_max - col_min + 1), FRAME_GRID_HEIGHT);
	}
}

void KeysContentWidget::MousePopupMenu(int x, int y)
{
	wxMenu menu;

	menu.Append(Menu_CreateClassicTween, entries[Menu_CreateClassicTween].text[currLanguage]);
	menu.Append(Menu_DeleteClassicTween, entries[Menu_DeleteClassicTween].text[currLanguage]);
	menu.AppendSeparator();
	menu.Append(Menu_InsertFrame, entries[Menu_InsertFrame].text[currLanguage]);
	menu.Append(Menu_DeleteFrame, entries[Menu_DeleteFrame].text[currLanguage]);
	menu.AppendSeparator();
	menu.Append(Menu_InsertKeyFrame, entries[Menu_InsertKeyFrame].text[currLanguage]);
	menu.Append(Menu_DeleteKeyFrame, entries[Menu_DeleteKeyFrame].text[currLanguage]);

	PopupMenu(&menu, x, y);	
}

void KeysContentWidget::OnCreateClassicTween(wxCommandEvent& event)
{
	KeyFrame* keyFrame = QueryKeyFrameByPos();
	keyFrame->SetClassicTween(true);
	Refresh(true);
}

void KeysContentWidget::OnDeleteClassicTween(wxCommandEvent& event)
{
	KeyFrame* keyFrame = QueryKeyFrameByPos();
	keyFrame->SetClassicTween(false);
	Refresh(true);
}

void KeysContentWidget::OnInsertFrame(wxCommandEvent& event)
{
	OnInsertFrame();
}

void KeysContentWidget::OnDeleteFrame(wxCommandEvent& event)
{
	OnDeleteFrame();
}

void KeysContentWidget::OnInsertKeyFrame(wxCommandEvent& event)
{
	int col;
	Layer* layer = get_curr_layer(col);
	layer->InsertKeyFrame(col + 1);
}

void KeysContentWidget::OnDeleteKeyFrame(wxCommandEvent& event)
{
	int col;
	Layer* layer = get_curr_layer(col);
	layer->RemoveKeyFrame(col + 1);
}

void KeysContentWidget::OnUpdateCreateClassicTween(wxUpdateUIEvent& event)
{
	KeyFrame* keyFrame = QueryKeyFrameByPos();
	if (keyFrame)
	{
		if (keyFrame->HasClassicTween())
			event.Enable(false);
		else
			event.Enable(true);
	}
	else
	{
		event.Enable(false);
	}
}

void KeysContentWidget::OnUpdateDeleteClassicTween(wxUpdateUIEvent& event)
{
	KeyFrame* keyFrame = QueryKeyFrameByPos();
	if (keyFrame)
	{
		if (keyFrame->HasClassicTween())
			event.Enable(true);
		else
			event.Enable(false);
	}
	else
	{
		event.Enable(false);
	}
}

void KeysContentWidget::OnUpdateInsertFrame(wxUpdateUIEvent& event)
{
}

void KeysContentWidget::OnUpdateDeleteFrame(wxUpdateUIEvent& event)
{
	if (IsPosOnKeyFrame()) event.Enable(false);
	else event.Enable(true);
}

void KeysContentWidget::OnUpdateInsertKeyFrame(wxUpdateUIEvent& event)
{
//	event.Enable(false);
}

void KeysContentWidget::OnUpdateDeleteKeyFrame(wxUpdateUIEvent& event)
{
	int col;
	Layer* layer = get_curr_layer(col);
	event.Enable(layer->IsKeyFrame(col + 1));
}

void KeysContentWidget::OnInsertFrame(wxKeyEvent& event)
{
	OnInsertFrame();
}

void KeysContentWidget::OnDeleteFrame(wxKeyEvent& event)
{
	OnDeleteFrame();
}

void KeysContentWidget::OnInsertFrame()
{
	int col;
	Layer* layer = get_curr_layer(col);
	layer->InsertNullFrame(col + 1);
}

void KeysContentWidget::OnDeleteFrame()
{
	int col;
	Layer* layer = get_curr_layer(col);
	layer->RemoveNullFrame(col + 1);
}

} // eanim