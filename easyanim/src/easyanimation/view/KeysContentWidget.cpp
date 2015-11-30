#include "KeysContentWidget.h"
#include "KeysPanel.h"

#include "frame/Controller.h"
#include "dataset/KeyFrame.h"
#include "dataset/Layer.h"
#include "view/StagePanel.h"

#include <wx/dcbuffer.h>

namespace eanim
{

BEGIN_EVENT_TABLE(KeysContentWidget, wxPanel)
	EVT_PAINT(KeysContentWidget::onPaint)
	EVT_ERASE_BACKGROUND(KeysContentWidget::onEraseBackground)
	EVT_SIZE(KeysContentWidget::onSize)
	EVT_MOUSE_EVENTS(KeysContentWidget::onMouse)
	EVT_KEY_DOWN(KeysContentWidget::OnKeyDown)
	EVT_KEY_UP(KeysContentWidget::OnKeyUp)

	EVT_MENU(Menu_CreateClassicTween, KeysContentWidget::onCreateClassicTween)
	EVT_MENU(Menu_DeleteClassicTween, KeysContentWidget::onDeleteClassicTween)
	EVT_MENU(Menu_InsertFrame, KeysContentWidget::onInsertFrame)
	EVT_MENU(Menu_DeleteFrame, KeysContentWidget::onDeleteFrame)
	EVT_MENU(Menu_InsertKeyFrame, KeysContentWidget::onInsertKeyFrame)
	EVT_MENU(Menu_DeleteKeyFrame, KeysContentWidget::onDeleteKeyFrame)

	EVT_UPDATE_UI(Menu_CreateClassicTween, KeysContentWidget::onUpdateCreateClassicTween)
	EVT_UPDATE_UI(Menu_DeleteClassicTween, KeysContentWidget::onUpdateDeleteClassicTween)
	EVT_UPDATE_UI(Menu_InsertFrame, KeysContentWidget::onUpdateInsertFrame)
	EVT_UPDATE_UI(Menu_DeleteFrame, KeysContentWidget::onUpdateDeleteFrame)
	EVT_UPDATE_UI(Menu_InsertKeyFrame, KeysContentWidget::onUpdateInsertKeyFrame)
	EVT_UPDATE_UI(Menu_DeleteKeyFrame, KeysContentWidget::onUpdateDeleteKeyFrame)

	EVT_HOTKEY(Hot_InsertFrame, KeysContentWidget::onInsertFrame)
	EVT_HOTKEY(Hot_DeleteFrame, KeysContentWidget::onDeleteFrame)
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

KeysContentWidget::KeysContentWidget(wxWindow* parent, Controller* ctrl, const LayersMgr& layers)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE)
	, m_editop(ctrl)
	, m_layers(layers)
{
	m_curr_layer = m_curr_frame = -1;

//  	RegisterHotKey(Hot_InsertFrame, 0, VK_ADD);
//  	RegisterHotKey(Hot_DeleteFrame, 0, VK_SUBTRACT);

	RegisterHotKey(Hot_InsertFrame, 0, VK_MULTIPLY);
	RegisterHotKey(Hot_DeleteFrame, 0, VK_DIVIDE);
}

void KeysContentWidget::Notify(int sj_id, void* ud)
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

void KeysContentWidget::onSize(wxSizeEvent& event)
{
	Refresh(true);
}

void KeysContentWidget::onPaint(wxPaintEvent& event)
{
	wxBufferedPaintDC dc(this);

	drawBackground(dc);
	drawLayersDataBg(dc);
	drawSelected(dc);
	drawLayersDataFlag(dc);
	drawCurrPosFlag(dc);
}

void KeysContentWidget::onEraseBackground(wxEraseEvent& event)
{
}

void KeysContentWidget::onMouse(wxMouseEvent& event)
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
		m_ctrl->GetStagePanel()->Undo();
	} else if (m_keys_state.GetKeyState(WXK_CONTROL) && (key_code == 'y' || key_code == 'Y')) {
		m_ctrl->GetStagePanel()->Redo();
	}
}

void KeysContentWidget::OnKeyUp(wxKeyEvent& event)
{
	int key_code = event.GetKeyCode();
	m_keys_state.OnKeyUp(key_code);
}

KeyFrame* KeysContentWidget::queryKeyFrameByPos() const
{
	int row, col;
	m_ctrl->GetKeysPanel()->GetSelectPos(row, col);
	if (row == -1 || col == -1) 
		return NULL;

	LayersMgr& layers = m_ctrl->GetLayers();
	Layer* layer = layers.GetLayer(layers.Size() - row - 1);
	if (!layer) 
		return NULL;
	else 
		return layer->GetCurrKeyFrame(col + 1);
}

bool KeysContentWidget::isPosOnKeyFrame() const
{
	int row, col;
	m_ctrl->GetKeysPanel()->GetSelectPos(row, col);
	if (row == -1 || col == -1) 
		return false;

	LayersMgr& layers = m_ctrl->GetLayers();
	Layer* layer = layers.GetLayer(layers.Size() - row - 1);
	if (!layer) 
		return false;
	else 
	{
		const std::map<int, KeyFrame*>& frames = layer->GetAllFrames();
		return frames.find(col + 1) != frames.end();
	}
}

void KeysContentWidget::drawBackground(wxBufferedPaintDC& dc)
{
	// background
	dc.SetPen(wxPen(LIGHT_GRAY));
	dc.SetBrush(wxBrush(LIGHT_GRAY));
	dc.DrawRectangle(GetSize());

	const size_t size = m_ctrl->GetLayers().Size();
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

void KeysContentWidget::drawLayersDataBg(wxBufferedPaintDC& dc)
{
	LayersMgr& layers = m_ctrl->GetLayers();
	for (size_t i = 0, n = layers.Size(); i < n; ++i)
	{
		size_t storeIndex = n - i - 1;
		const std::map<int, KeyFrame*>& frames = layers.GetLayer(storeIndex)->GetAllFrames();
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

void KeysContentWidget::drawLayersDataFlag(wxBufferedPaintDC& dc)
{
	LayersMgr& layers = m_ctrl->GetLayers();
	for (size_t i = 0, n = layers.Size(); i < n; ++i)
	{
		size_t storeIndex = n - i - 1;
		const std::map<int, KeyFrame*>& frames = layers.GetLayer(storeIndex)->GetAllFrames();
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

void KeysContentWidget::drawCurrPosFlag(wxBufferedPaintDC& dc)
{
	const float x = FRAME_GRID_WIDTH * (m_ctrl->frame() - 0.5f);
	dc.SetPen(DARK_RED);
	dc.DrawLine(x, 0, x, FRAME_GRID_HEIGHT * m_ctrl->GetLayers().Size());
}

void KeysContentWidget::drawSelected(wxBufferedPaintDC& dc)
{
	int row, col;
	m_ctrl->GetKeysPanel()->GetSelectPos(row, col);
	if (row != -1 && col != -1)
	{
		dc.SetPen(wxPen(DARK_BLUE));
		dc.SetBrush(wxBrush(DARK_BLUE));
		dc.DrawRectangle(FRAME_GRID_WIDTH * col, FRAME_GRID_HEIGHT * row, FRAME_GRID_WIDTH, FRAME_GRID_HEIGHT);
	}

	int col_min, col_max;
	m_ctrl->GetKeysPanel()->GetSelectRegion(row, col_min, col_max);
	if (col_min != -1 && col_max != -1 && col_min != col_max)
	{
		dc.SetPen(wxPen(DARK_BLUE));
		dc.SetBrush(wxBrush(DARK_BLUE));
		dc.DrawRectangle(FRAME_GRID_WIDTH * col_min, FRAME_GRID_HEIGHT * row, FRAME_GRID_WIDTH * (col_max - col_min + 1), FRAME_GRID_HEIGHT);
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

void KeysContentWidget::onCreateClassicTween(wxCommandEvent& event)
{
	KeyFrame* keyFrame = queryKeyFrameByPos();
	keyFrame->SetClassicTween(true);
	Refresh(true);
}

void KeysContentWidget::onDeleteClassicTween(wxCommandEvent& event)
{
	KeyFrame* keyFrame = queryKeyFrameByPos();
	keyFrame->SetClassicTween(false);
	Refresh(true);
}

void KeysContentWidget::onInsertFrame(wxCommandEvent& event)
{
	onInsertFrame();
}

void KeysContentWidget::onDeleteFrame(wxCommandEvent& event)
{
	onDeleteFrame();
}

void KeysContentWidget::onInsertKeyFrame(wxCommandEvent& event)
{
	int row, col;
	m_ctrl->GetKeysPanel()->GetSelectPos(row, col);
	if (row != -1 && col != -1)
	{
		LayersMgr& layers = m_ctrl->GetLayers();
		size_t index = layers.Size() - row - 1;
		Layer* layer = layers.GetLayer(index);
		layer->InsertKeyFrame(col + 1);
		m_ctrl->GetKeysPanel()->Refresh(true);
	}
}

void KeysContentWidget::onDeleteKeyFrame(wxCommandEvent& event)
{
	int row, col;
	m_ctrl->GetKeysPanel()->GetSelectPos(row, col);

	LayersMgr& layers = m_ctrl->GetLayers();
	size_t index = layers.Size() - row - 1;
	Layer* layer = layers.GetLayer(index);
	layer->RemoveKeyFrame(col + 1);
	m_ctrl->GetKeysPanel()->Refresh(true);
}

void KeysContentWidget::onUpdateCreateClassicTween(wxUpdateUIEvent& event)
{
	KeyFrame* keyFrame = queryKeyFrameByPos();
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

void KeysContentWidget::onUpdateDeleteClassicTween(wxUpdateUIEvent& event)
{
	KeyFrame* keyFrame = queryKeyFrameByPos();
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

void KeysContentWidget::onUpdateInsertFrame(wxUpdateUIEvent& event)
{
}

void KeysContentWidget::onUpdateDeleteFrame(wxUpdateUIEvent& event)
{
	if (isPosOnKeyFrame()) event.Enable(false);
	else event.Enable(true);
}

void KeysContentWidget::onUpdateInsertKeyFrame(wxUpdateUIEvent& event)
{
//	event.Enable(false);
}

void KeysContentWidget::onUpdateDeleteKeyFrame(wxUpdateUIEvent& event)
{
	int row, col;
	m_ctrl->GetKeysPanel()->GetSelectPos(row, col);
	if (row == -1 || col == -1) return;

	LayersMgr& layers = m_ctrl->GetLayers();
	size_t index = layers.Size() - row - 1;
	Layer* layer = layers.GetLayer(index);
	if (layer->IsKeyFrame(col + 1)) 
		event.Enable(true);
	else 
		event.Enable(false);
}

void KeysContentWidget::onInsertFrame(wxKeyEvent& event)
{
	onInsertFrame();
}

void KeysContentWidget::onDeleteFrame(wxKeyEvent& event)
{
	onDeleteFrame();
}

void KeysContentWidget::onInsertFrame()
{
	int row, col;
	m_ctrl->GetKeysPanel()->GetSelectPos(row, col);
	if (row != -1 && col != -1)
	{
		LayersMgr& layers = m_ctrl->GetLayers();
		size_t index = layers.Size() - row - 1;
		Layer* layer = layers.GetLayer(index);
		layer->InsertNullFrame(col + 1);
		m_ctrl->GetKeysPanel()->Refresh(true);
	}
}

void KeysContentWidget::onDeleteFrame()
{
	int row, col;
	m_ctrl->GetKeysPanel()->GetSelectPos(row, col);
	if (row != -1 && col != -1)
	{
		LayersMgr& layers = m_ctrl->GetLayers();
		size_t index = layers.Size() - row - 1;
		Layer* layer = layers.GetLayer(index);
		layer->RemoveNullFrame(col + 1);
		m_ctrl->GetKeysPanel()->Refresh(true);
	}
}

} // eanim