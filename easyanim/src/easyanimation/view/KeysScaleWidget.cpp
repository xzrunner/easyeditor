#include "KeysScaleWidget.h"
#include "KeysPanel.h"
#include "config.h"

#include "dataset/LayersMgr.h"
#include "dataset/DataMgr.h"
#include "message/messages.h"

#include <wx/dcbuffer.h>

namespace eanim
{

BEGIN_EVENT_TABLE(KeysScaleWidget, wxPanel)
	EVT_PAINT(KeysScaleWidget::OnPaint)
	EVT_ERASE_BACKGROUND(KeysScaleWidget::OnEraseBackground)
	EVT_SIZE(KeysScaleWidget::OnSize)
	EVT_MOUSE_EVENTS(KeysScaleWidget::OnMouse)
END_EVENT_TABLE()

static const int DIVISION_HEIGHT = 4;
static const int TEXT_Y = 4;

KeysScaleWidget::KeysScaleWidget(wxWindow* parent)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(-1, FRAME_GRID_HEIGHT), wxBORDER_NONE)
	, m_curr_frame(-1)
{
//	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
//	// same with "EVT_ERASE_BACKGROUND(KeysScaleWidget::onEraseBackground)"

	RegistSubject(SetCurrFrameSJ::Instance());
}

void KeysScaleWidget::OnSize(wxSizeEvent& event)
{
	Refresh(true);
}

void KeysScaleWidget::OnPaint(wxPaintEvent& event)
{
	wxBufferedPaintDC dc(this);

	// background
	dc.SetPen(wxPen(MEDIUM_GRAY));
	dc.SetBrush(wxBrush(MEDIUM_GRAY));
	dc.DrawRectangle(GetSize());

	// curr pos
	int curr_pos = m_curr_frame + 1;

	dc.SetPen(wxPen(DARK_RED));
	dc.SetBrush(wxBrush(MEDIUM_RED));
	dc.DrawRectangle(FRAME_GRID_WIDTH * (curr_pos - 1), 2, FRAME_GRID_WIDTH + 1, FRAME_GRID_HEIGHT - 2);
	dc.DrawLine(FRAME_GRID_WIDTH * (curr_pos - 0.5f), FRAME_GRID_HEIGHT, FRAME_GRID_WIDTH * (curr_pos - 0.5f), 100);
	if (curr_pos % 5 != 0)
	{
		wxSize size = dc.GetTextExtent(wxString::Format(wxT("%d"), curr_pos));
		dc.DrawText(wxString::Format(wxT("%d"), curr_pos), 
			FRAME_GRID_WIDTH * (curr_pos - 0.5f) - size.GetWidth() / 2, TEXT_Y);
	}

	// scale
	dc.SetPen(wxPen(TEXT_COLOR));
	for (int i = 1; i <= MAX_FRAME_COUNT; ++i)
	{
		dc.DrawLine(FRAME_GRID_WIDTH * i, FRAME_GRID_HEIGHT, FRAME_GRID_WIDTH * i, FRAME_GRID_HEIGHT - DIVISION_HEIGHT);
		if (i == 1 || i % 5 == 0)
		{
			wxSize size = dc.GetTextExtent(wxString::Format(wxT("%d"), i));
			dc.DrawText(wxString::Format(wxT("%d"), i), 
				FRAME_GRID_WIDTH * (i - 0.5f) - size.GetWidth() / 2, TEXT_Y);
		}
	}
}

void KeysScaleWidget::OnEraseBackground(wxEraseEvent& event)
{

}

void KeysScaleWidget::OnMouse(wxMouseEvent& event)
{
	if (event.LeftDown() || event.Dragging())
	{
		int frame = QueryGridByPos(event.GetX());
		SetCurrFrameSJ::Instance()->Set(-1, frame);
	}
}

void KeysScaleWidget::OnNotify(int sj_id, void* ud)
{
	if (sj_id == MSG_SET_CURR_FRAME) {
		SetCurrFrameSJ::CurrFrame* cf = (SetCurrFrameSJ::CurrFrame*)ud;
		if (cf->frame != -1 && cf->frame != m_curr_frame) {
			m_curr_frame = cf->frame;
			Refresh();
		}
	}
}

int KeysScaleWidget::QueryGridByPos(float x) const
{
	return std::min(DataMgr::Instance()->GetLayers().GetMaxFrame(), (int)(x / FRAME_GRID_WIDTH) + 1);
}

} // eanim