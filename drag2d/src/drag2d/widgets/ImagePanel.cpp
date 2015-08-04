#include "ImagePanel.h"

namespace d2d
{

BEGIN_EVENT_TABLE(ImagePanel, wxPanel)
	EVT_PAINT(ImagePanel::OnPaint)
	EVT_SIZE(ImagePanel::OnSize)
END_EVENT_TABLE()

ImagePanel::ImagePanel(wxWindow* parent, const std::string& filepath,
					   int width, int height)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(width, height))
{
	m_image.LoadFile(filepath);
	m_resized = wxBitmap(m_image.Scale(width, height));
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

	dc.DrawBitmap( m_resized, 0, 0, false );
}

}