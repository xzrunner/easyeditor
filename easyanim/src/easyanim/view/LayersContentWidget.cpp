#include "LayersContentWidget.h"
#include "LayersPanel.h"
#include "Utility.h"
#include "dataset/Layer.h"
#include "dataset/LayersMgr.h"


namespace eanim
{

BEGIN_EVENT_TABLE(LayersContentWidget, wxPanel)
	EVT_PAINT(LayersContentWidget::onPaint)
	EVT_ERASE_BACKGROUND(LayersContentWidget::onEraseBackground)
	EVT_SIZE(LayersContentWidget::onSize)
	EVT_MOUSE_EVENTS(LayersContentWidget::onMouse)
END_EVENT_TABLE()

LayersContentWidget::LayersContentWidget(LayersPanel* parent, int id, LayersMgr* layersMgr)
	: wxPanel(parent, id, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE)
{
	m_parent = parent;
	m_layersMgr = layersMgr;
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

	const size_t size = m_layersMgr->size();
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
	int select = m_parent->getSelectedLayer();
	if (select != -1)
	{
		int screenIndex = m_layersMgr->size() - select - 1;
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
		Layer* layer = m_layersMgr->getLayer(storeIndex);
		dc.DrawText(layer->getName(), 5, FRAME_GRID_HEIGHT * i);
	}
}

void LayersContentWidget::onEraseBackground(wxEraseEvent& event)
{

}

void LayersContentWidget::onMouse(wxMouseEvent& event)
{
	static bool isDragOpen = false;

	if (event.LeftDown())
	{
		int screenIndex = event.GetY() / FRAME_GRID_HEIGHT;
		m_parent->setSelectedLayer(m_layersMgr->size() - screenIndex - 1);
		if (screenIndex < m_layersMgr->size()) isDragOpen = true;
	}
	else if (event.LeftUp())
	{
		isDragOpen = false;
		if (m_dragFlagLine != -1) 
		{
			int from = m_parent->getSelectedLayer(),
				to = m_layersMgr->size() - m_dragFlagLine;
			if (to == from || to == from + 1)
				;
			else
			{
				if (to > from) --to;
				m_layersMgr->changeLayerOrder(from, to);
				m_parent->refreshKeysPanel();
			}
			Refresh();
		}
		m_dragFlagLine = -1;
	}
	else if (event.Dragging())
	{
		const int size = m_layersMgr->size();
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
		int layerIndex = m_layersMgr->size() - screenIndex - 1;
		if (layerIndex < m_layersMgr->size())
		{
			Layer* layer = m_layersMgr->getLayer(layerIndex);
			
			wxPoint pos(GetScreenPosition() + wxPoint(event.GetX(), event.GetY()));
			ee::SetValueDialog dlg(this, wxT("Set layer's name"), layer->getName(), pos);
			if (dlg.ShowModal() == wxID_OK)
			{
				layer->setName(dlg.getText().ToStdString());
				Refresh();
			}
		}
	}
}

} // eanim