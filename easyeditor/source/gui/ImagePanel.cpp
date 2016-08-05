#include "ImagePanel.h"

namespace ee
{

BEGIN_EVENT_TABLE(ImagePanel, wxPanel)
	EVT_PAINT(ImagePanel::OnPaint)
END_EVENT_TABLE()

ImagePanel::ImagePanel(wxWindow* parent, const std::string& filepath,
					   float max_len)
	: wxPanel(parent, wxID_ANY)
{
	m_image.LoadFile(filepath);
	Resize(max_len);
}

void ImagePanel::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	Draw(dc);
}

void ImagePanel::Draw(wxDC& dc) const
{
	dc.DrawBitmap(m_resized, 0, 0, false);
}

void ImagePanel::Resize(float max_len)
{
	float w = m_image.GetWidth(),
		  h = m_image.GetHeight();
	if (w <= max_len && h <= max_len) {
		m_resized = wxBitmap(m_image);
		SetInitialSize(wxSize(w, h));
	} else {
		float scale = std::min(max_len / w, max_len / h);
		m_resized = wxBitmap(m_image.Scale(w * scale, h * scale));
		SetInitialSize(wxSize(w * scale, h * scale));
	}
}

}