#include "LayersContentWidget.h"
#include "config.h"
#include "KeysPanel.h"

#include "dataset/Layer.h"
#include "dataset/LayersMgr.h"
#include "dataset/DataMgr.h"
#include "message/messages.h"

namespace eanim
{

BEGIN_EVENT_TABLE(LayersContentWidget, wxPanel)
	EVT_PAINT(LayersContentWidget::OnPaint)
	EVT_ERASE_BACKGROUND(LayersContentWidget::OnEraseBackground)
	EVT_SIZE(LayersContentWidget::OnSize)
	EVT_MOUSE_EVENTS(LayersContentWidget::OnMouse)
END_EVENT_TABLE()

static const int DRAG_FLAG_SIZE = 3;
static const int DRAG_FLAG_RADIUS = 4;

static const int FLAG_RADIUS = 8;
static const int FLAG_EDITABLE_X = 80;
static const int FLAG_VISIBLE_X = 120;

LayersContentWidget::LayersContentWidget(wxWindow* parent)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE)
{
	m_drag_flag_line = -1;
	m_curr_layer = -1;

	RegistSubject(SetSelectedSJ::Instance());
	RegistSubject(RemoveLayerSJ::Instance());
}

wxCoord LayersContentWidget::OnGetRowHeight(size_t row) const
{
	return FRAME_GRID_HEIGHT;
}

int LayersContentWidget::GetNonOrientationTargetSize() const
{
	return FRAME_GRID_HEIGHT * 20;
}

wxOrientation LayersContentWidget::GetOrientation() const
{
	return wxVERTICAL;
}

void LayersContentWidget::OnSize(wxSizeEvent& event)
{
	Refresh(true);
}

void LayersContentWidget::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);

	int size = DataMgr::Instance()->GetLayers().Size();
	const float width = GetSize().x;

	// background
	dc.SetPen(wxPen(LIGHT_GRAY));
	dc.SetBrush(wxBrush(LIGHT_GRAY));
	dc.DrawRectangle(GetSize());

	// white grids
	dc.SetPen(*wxWHITE_PEN);
	dc.SetBrush(*wxWHITE_BRUSH);
	dc.DrawRectangle(0, 0, width, FRAME_GRID_HEIGHT * size);

	// grid lines
	dc.SetPen(wxPen(MEDIUM_GRAY));
	for (size_t i = 0; i <= size; ++i)
	{
		float y = FRAME_GRID_HEIGHT * i;
		dc.DrawLine(0, y, width, y);
	}

	// selected
	if (m_curr_layer >= 0)
	{
		assert(m_curr_layer < DataMgr::Instance()->GetLayers().Size());
		int idx = size - m_curr_layer - 1;
		dc.SetPen(wxPen(MEDIUM_BLUE));
		dc.SetBrush(wxBrush(MEDIUM_BLUE));
		dc.DrawRectangle(0, FRAME_GRID_HEIGHT * idx, width, FRAME_GRID_HEIGHT);
	}

	// drag flag
	if (m_drag_flag_line != -1)
	{
		float y = FRAME_GRID_HEIGHT * m_drag_flag_line;
		dc.SetPen(wxPen(wxColour(0, 0, 0), DRAG_FLAG_SIZE));
		dc.SetBrush(*wxWHITE_BRUSH);
		dc.DrawCircle(FRAME_GRID_HEIGHT, y, DRAG_FLAG_RADIUS);
		dc.DrawLine(FRAME_GRID_HEIGHT + DRAG_FLAG_RADIUS, y, GetSize().x, y);
	}

	// layers' name
	dc.SetPen(wxPen(TEXT_COLOR));
	for (size_t i = 0; i < size; ++i)
	{
		size_t idx = size - i - 1;
		Layer* layer = DataMgr::Instance()->GetLayers().GetLayer(idx);
		dc.DrawText(layer->GetName(), 5, FRAME_GRID_HEIGHT * i);

		dc.SetPen(*wxBLACK_PEN);
		if (layer->IsEditable()) {
			dc.SetBrush(*wxBLACK_BRUSH);
		} else {
			dc.SetBrush(*wxWHITE_BRUSH);
		}
		dc.DrawRectangle(FLAG_EDITABLE_X, FRAME_GRID_HEIGHT * i + (FRAME_GRID_HEIGHT - FLAG_RADIUS*2) * 0.5, FLAG_RADIUS*2, FLAG_RADIUS*2);

		if (layer->IsVisible()) {
			dc.SetBrush(*wxBLACK_BRUSH);
		} else {
			dc.SetBrush(*wxWHITE_BRUSH);
		}
		dc.DrawCircle(FLAG_VISIBLE_X, FRAME_GRID_HEIGHT * i + FRAME_GRID_HEIGHT * 0.5f, FLAG_RADIUS);
	}
}

void LayersContentWidget::OnEraseBackground(wxEraseEvent& event)
{

}

void LayersContentWidget::OnMouse(wxMouseEvent& event)
{
	static bool is_drag_open = false;
	static int xpress = 0, ypress = 0;

	int size = DataMgr::Instance()->GetLayers().Size();

	if (event.LeftDown())
	{
		xpress = event.GetX();
		ypress = event.GetY();

		unsigned int screen_idx = event.GetY() / FRAME_GRID_HEIGHT;
		int layer = size - screen_idx - 1;
		if (layer >= 0) {
			SetSelectedSJ::Instance()->Set(layer, -1);
			if (screen_idx < size) {
				is_drag_open = true;
			}
		}
	}
	else if (event.LeftUp())
	{
		if (xpress == event.GetX() && ypress == event.GetY())
		{
			unsigned int screen_idx = event.GetY() / FRAME_GRID_HEIGHT;
			int layer_idx = size - screen_idx - 1;
	
			int x = event.GetX();
			Layer* layer = DataMgr::Instance()->GetLayers().GetLayer(layer_idx);
			if (layer && x > FLAG_EDITABLE_X && x < FLAG_EDITABLE_X + FLAG_RADIUS * 2) {
				layer->SetEditable(!layer->IsEditable());
				Refresh(true);
			} else if (layer && x > FLAG_VISIBLE_X - FLAG_RADIUS && x < FLAG_VISIBLE_X + FLAG_RADIUS) {
				layer->SetVisible(!layer->IsVisible());
				Refresh(true);
			}
		}

		is_drag_open = false;
		if (m_drag_flag_line != -1) 
		{
			int from = m_curr_layer,
				to = size - m_drag_flag_line;
			if (to == from || to == from + 1)
				;
			else
			{
				if (to > from) --to;
				ReorderLayerSJ::Instance()->Reorder(from, to);
				SetSelectedSJ::Instance()->Set(to, -1);
			}
		}
		m_drag_flag_line = -1;
	}
	else if (event.Dragging())
	{
		if (is_drag_open && size != 0)
		{
			int newDragLine = m_drag_flag_line;
			int y = event.GetY();
			if (y < 0) newDragLine = 0;
			else if (y > FRAME_GRID_HEIGHT * size) newDragLine = size;
			else newDragLine = (float)y / FRAME_GRID_HEIGHT + 0.5f;
			if (newDragLine != m_drag_flag_line) 
			{
				m_drag_flag_line = newDragLine;
				Refresh(true);
			}
		}
	}
	else if (event.LeftDClick())
	{
		int screen_idx = event.GetY() / FRAME_GRID_HEIGHT;
		int layer_idx = size - screen_idx - 1;
		if (layer_idx < size)
		{
			Layer* layer = DataMgr::Instance()->GetLayers().GetLayer(layer_idx);
			
			wxPoint pos(GetScreenPosition() + wxPoint(event.GetX(), event.GetY()));
			d2d::SetValueDialog dlg(this, wxT("Set layer's name"), layer->GetName(), pos);
			if (dlg.ShowModal() == wxID_OK)
			{
				layer->SetName(dlg.getText().ToStdString());
				Refresh(true);
			}
		}
	}
}

void LayersContentWidget::OnNotify(int sj_id, void* ud)
{
	switch (sj_id)
	{
	case MSG_SET_CURR_FRAME:
		{
			SetSelectedSJ::Position* cf = (SetSelectedSJ::Position*)ud;
			if (cf->layer != -1 && cf->layer != m_curr_layer) {
				m_curr_layer = cf->layer;
				Refresh();
			}
		}
		break;
	case MSG_REMOVE_LAYER:
		{
			int layer = *(int*)ud;
			if (layer == m_curr_layer) {
				m_curr_layer = -1;
				Refresh();
			}
		}
		break;
	}
}

} // eanim