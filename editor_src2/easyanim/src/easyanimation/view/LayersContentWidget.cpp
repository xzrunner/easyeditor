#include "LayersContentWidget.h"
#include "Utility.h"
#include "KeysPanel.h"

#include "frame/Context.h"
#include "dataset/Layer.h"

namespace eanim
{

BEGIN_EVENT_TABLE(LayersContentWidget, wxPanel)
	EVT_PAINT(LayersContentWidget::onPaint)
	EVT_ERASE_BACKGROUND(LayersContentWidget::onEraseBackground)
	EVT_SIZE(LayersContentWidget::onSize)
	EVT_MOUSE_EVENTS(LayersContentWidget::onMouse)
END_EVENT_TABLE()

LayersContentWidget::LayersContentWidget(wxWindow* parent)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE)
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
	Refresh();
}

void LayersContentWidget::onPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);

	const size_t size = Context::Instance()->layers.size();
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
	if (Context::Instance()->layer() != -1)
	{
		int screenIndex = Context::Instance()->layers.size() 
			- Context::Instance()->layer() - 1;
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
		Layer* layer = Context::Instance()->layers.getLayer(storeIndex);
		dc.DrawText(layer->name, 5, FRAME_GRID_HEIGHT * i);
	}
}

void LayersContentWidget::onEraseBackground(wxEraseEvent& event)
{

}

void LayersContentWidget::onMouse(wxMouseEvent& event)
{
	static bool isDragOpen = false;

	const unsigned int size = Context::Instance()->layers.size();

	if (event.LeftDown())
	{
		Context* context = Context::Instance();
		unsigned int screenIndex = event.GetY() / FRAME_GRID_HEIGHT;
		int layer = size - screenIndex - 1;
		context->setCurrFrame(layer, context->frame());
		if (screenIndex < size) 
			isDragOpen = true;
	}
	else if (event.LeftUp())
	{
		isDragOpen = false;
		if (m_dragFlagLine != -1) 
		{
			int from = Context::Instance()->layer(),
				to = size - m_dragFlagLine;
			if (to == from || to == from + 1)
				;
			else
			{
				if (to > from) --to;
				Context::Instance()->layers.changeLayerOrder(from, to);
				Context::Instance()->keysPanel->Refresh();
			}
			Refresh();
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
				Refresh();
			}
		}
	}
	else if (event.LeftDClick())
	{
		int screenIndex = event.GetY() / FRAME_GRID_HEIGHT;
		int layerIndex = size - screenIndex - 1;
		if (layerIndex < size)
		{
			Layer* layer = Context::Instance()->layers.getLayer(layerIndex);
			
			wxPoint pos(GetScreenPosition() + wxPoint(event.GetX(), event.GetY()));
			d2d::SetValueDialog dlg(this, wxT("Set layer's name"), layer->name, pos);
			if (dlg.ShowModal() == wxID_OK)
			{
				layer->name = dlg.getText().ToStdString();
				Refresh();
			}
		}
	}
}

} // eanim