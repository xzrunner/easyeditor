#include "LayersContentWidget.h"
#include "Utility.h"
#include "KeysPanel.h"

#include "frame/Controller.h"
#include "dataset/Layer.h"

namespace eanim
{

BEGIN_EVENT_TABLE(LayersContentWidget, wxPanel)
	EVT_PAINT(LayersContentWidget::onPaint)
	EVT_ERASE_BACKGROUND(LayersContentWidget::onEraseBackground)
	EVT_SIZE(LayersContentWidget::onSize)
	EVT_MOUSE_EVENTS(LayersContentWidget::onMouse)
END_EVENT_TABLE()

static const int FLAG_RADIUS = 8;
static const int FLAG_EDITABLE_X = 80;
static const int FLAG_VISIBLE_X = 120;

LayersContentWidget::LayersContentWidget(wxWindow* parent, Controller* ctrl)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE)
	, m_ctrl(ctrl)
{
	m_dragFlagLine = -1;
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

void LayersContentWidget::onSize(wxSizeEvent& event)
{
	Refresh(true);
}

void LayersContentWidget::onPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);

	const size_t size = m_ctrl->GetLayers().size();
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
	if (m_ctrl->layer() != -1)
	{
		int screenIndex = m_ctrl->GetLayers().size() - m_ctrl->layer() - 1;
		dc.SetPen(wxPen(MEDIUM_BLUE));
		dc.SetBrush(wxBrush(MEDIUM_BLUE));
		dc.DrawRectangle(0, FRAME_GRID_HEIGHT * screenIndex, width, FRAME_GRID_HEIGHT);
	}

	// drag flag
	if (m_dragFlagLine != -1)
	{
		float y = FRAME_GRID_HEIGHT * m_dragFlagLine;
		dc.SetPen(wxPen(wxColour(0, 0, 0), DRAG_FLAG_SIZE));
		dc.SetBrush(*wxWHITE_BRUSH);
		dc.DrawCircle(FRAME_GRID_HEIGHT, y, DRAG_FLAG_RADIUS);
		dc.DrawLine(FRAME_GRID_HEIGHT + DRAG_FLAG_RADIUS, y, GetSize().x, y);
	}

	// layers' name
	dc.SetPen(wxPen(TEXT_COLOR));
	for (size_t i = 0; i < size; ++i)
	{
		size_t storeIndex = size - i - 1;
		Layer* layer = m_ctrl->GetLayers().getLayer(storeIndex);
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

void LayersContentWidget::onEraseBackground(wxEraseEvent& event)
{

}

void LayersContentWidget::onMouse(wxMouseEvent& event)
{
	static bool isDragOpen = false;
	static int xpress = 0, ypress = 0;

	const unsigned int size = m_ctrl->GetLayers().size();

	if (event.LeftDown())
	{
		xpress = event.GetX();
		ypress = event.GetY();

		unsigned int screenIndex = event.GetY() / FRAME_GRID_HEIGHT;
		int layer = size - screenIndex - 1;
		if (layer >= 0) {
			m_ctrl->setCurrFrame(layer, m_ctrl->frame());
			m_ctrl->Refresh();
			if (screenIndex < size) 
				isDragOpen = true;
		}
	}
	else if (event.LeftUp())
	{
		if (xpress == event.GetX() && ypress == event.GetY())
		{
			unsigned int screenIndex = event.GetY() / FRAME_GRID_HEIGHT;
			int layerIndex = size - screenIndex - 1;
	
			int x = event.GetX();
			Layer* layer = m_ctrl->GetLayers().getLayer(layerIndex);
			if (layer && x > FLAG_EDITABLE_X && x < FLAG_EDITABLE_X + FLAG_RADIUS * 2) {
				layer->SetEditable(!layer->IsEditable());
				Refresh(true);
				m_ctrl->Refresh();
			} else if (layer && x > FLAG_VISIBLE_X - FLAG_RADIUS && x < FLAG_VISIBLE_X + FLAG_RADIUS) {
				layer->SetVisible(!layer->IsVisible());
				Refresh(true);
				m_ctrl->Refresh();
			}
		}

		isDragOpen = false;
		if (m_dragFlagLine != -1) 
		{
			int from = m_ctrl->layer(),
				to = size - m_dragFlagLine;
			if (to == from || to == from + 1)
				;
			else
			{
				if (to > from) --to;
				m_ctrl->GetLayers().changeLayerOrder(from, to);
				m_ctrl->Refresh();
			}
			Refresh(true);
		}
		m_dragFlagLine = -1;
	}
	else if (event.Dragging())
	{
		if (isDragOpen && size != 0)
		{
			int newDragLine = m_dragFlagLine;
			int y = event.GetY();
			if (y < 0) newDragLine = 0;
			else if (y > FRAME_GRID_HEIGHT * size) newDragLine = size;
			else newDragLine = (float)y / FRAME_GRID_HEIGHT + 0.5f;
			if (newDragLine != m_dragFlagLine) 
			{
				m_dragFlagLine = newDragLine;
				Refresh(true);
			}
		}
	}
	else if (event.LeftDClick())
	{
		int screenIndex = event.GetY() / FRAME_GRID_HEIGHT;
		int layerIndex = size - screenIndex - 1;
		if (layerIndex < size)
		{
			Layer* layer = m_ctrl->GetLayers().getLayer(layerIndex);
			
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

} // eanim