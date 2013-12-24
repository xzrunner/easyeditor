#include "KeysScaleWidget.h"
#include "Utility.h"

#include "frame/Context.h"

#include <wx/dcbuffer.h>

namespace eanim
{

BEGIN_EVENT_TABLE(KeysScaleWidget, wxPanel)
	EVT_PAINT(KeysScaleWidget::onPaint)
	EVT_ERASE_BACKGROUND(KeysScaleWidget::onEraseBackground)
	EVT_SIZE(KeysScaleWidget::onSize)
	EVT_MOUSE_EVENTS(KeysScaleWidget::onMouse)
END_EVENT_TABLE()

KeysScaleWidget::KeysScaleWidget(wxWindow* parent)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(-1, FRAME_GRID_HEIGHT), wxBORDER_NONE)
{
//	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
//	// same with "EVT_ERASE_BACKGROUND(KeysScaleWidget::onEraseBackground)"
}

void KeysScaleWidget::onSize(wxSizeEvent& event)
{
	Refresh();
}

void KeysScaleWidget::onPaint(wxPaintEvent& event)
{
	wxBufferedPaintDC dc(this);

	// background
	dc.SetPen(wxPen(MEDIUM_GRAY));
	dc.SetBrush(wxBrush(MEDIUM_GRAY));
	dc.DrawRectangle(GetSize());

	// curr pos
	const int currPos = Context::Instance()->currFrame;
	dc.SetPen(wxPen(DARK_RED));
	dc.SetBrush(wxBrush(MEDIUM_RED));
	dc.DrawRectangle(FRAME_GRID_WIDTH * (currPos - 1), 2, FRAME_GRID_WIDTH + 1, FRAME_GRID_HEIGHT - 2);
	dc.DrawLine(FRAME_GRID_WIDTH * (currPos - 0.5f), FRAME_GRID_HEIGHT, FRAME_GRID_WIDTH * (currPos - 0.5f), 100);
	if (currPos % 5 != 0)
	{
		wxSize size = dc.GetTextExtent(wxString::Format(wxT("%d"), currPos));
		dc.DrawText(wxString::Format(wxT("%d"), currPos), 
			FRAME_GRID_WIDTH * (currPos - 0.5f) - size.GetWidth() / 2, TEXT_Y);
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

void KeysScaleWidget::onEraseBackground(wxEraseEvent& event)
{

}

void KeysScaleWidget::onMouse(wxMouseEvent& event)
{
	if (event.LeftDown())
	{
		Context* context = Context::Instance();
		context->currFrame = queryGridByPos(event.GetX());
		context->stage->Refresh();
		context->keysPanel->Refresh();
	}
	else if (event.Dragging())
	{
		Context* context = Context::Instance();
		context->currFrame = queryGridByPos(event.GetX());
		context->stage->Refresh();
		context->keysPanel->Refresh();
	}
}

int KeysScaleWidget::queryGridByPos(float x) const
{
	return std::min(Context::Instance()->maxFrame, (int)(x / FRAME_GRID_WIDTH) + 1);
}

} // eanim