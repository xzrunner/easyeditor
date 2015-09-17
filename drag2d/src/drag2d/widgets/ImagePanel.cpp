#include "ImagePanel.h"

namespace d2d
{

BEGIN_EVENT_TABLE(ImagePanel, wxPanel)
	EVT_PAINT(ImagePanel::OnPaint)
	EVT_SIZE(ImagePanel::OnSize)
END_EVENT_TABLE()

ImagePanel::ImagePanel(wxWindow* parent, const std::string& filepath,
					   float max_len)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(max_len, max_len))
{
	m_image.LoadFile(filepath);
	Resize(max_len);
}

void ImagePanel::OnSize(wxSizeEvent& event)
{
	Refresh();
	event.Skip();
}

void ImagePanel::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	Draw(dc);
}

void ImagePanel::Draw(wxDC& dc) const
{
// 	int neww, newh;
// 	dc.GetSize( &neww, &newh );
// 
// 	if( neww != m_width || newh != m_height )
// 	{
// 		m_resized = wxBitmap(m_image.Scale( neww, newh /*, wxIMAGE_QUALITY_HIGH*/ ));
// 		m_width = neww;
// 		m_height = newh;
// 		dc.DrawBitmap( m_resized, 0, 0, false );
// 	}else{
// 		dc.DrawBitmap( m_resized, 0, 0, false );
// 	}

	dc.DrawBitmap(m_resized, 0, 0, false);
}

void ImagePanel::Resize(float max_len)
{
	float w = m_image.GetWidth(),
		h = m_image.GetHeight();
	if (w <= max_len && h <= max_len) {
		m_resized = wxBitmap(m_image);
	} else {
		float scale = std::min(max_len / w, max_len / h);
		m_resized = wxBitmap(m_image.Scale(w * scale, h * scale));
	}
}

}