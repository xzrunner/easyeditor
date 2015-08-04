#ifndef _DRAG2D_IMAGE_PANEL_H_
#define _DRAG2D_IMAGE_PANEL_H_

#include <wx/wx.h>

namespace d2d
{

class ImagePanel : public wxPanel
{
public:
	ImagePanel(wxWindow* parent, const std::string& filepath, 
		int width, int height);

private:
	void OnSize(wxSizeEvent& event);
	void OnPaint(wxPaintEvent& event);

	void Draw(wxDC& dc) const;

private:
	wxImage m_image;
	wxBitmap m_resized;

// 	mutable wxBitmap m_resized;
// 
// 	mutable int m_width, m_height;
	
	DECLARE_EVENT_TABLE()

}; // ImagePanel

}

#endif // _DRAG2D_IMAGE_PANEL_H_