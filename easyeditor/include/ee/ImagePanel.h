#ifndef _EASYEDITOR_IMAGE_PANEL_H_
#define _EASYEDITOR_IMAGE_PANEL_H_

#include <wx/wx.h>

namespace ee
{

class ImagePanel : public wxPanel
{
public:
	ImagePanel(wxWindow* parent, const std::string& filepath, float max_len);

private:
	void OnSize(wxSizeEvent& event);
	void OnPaint(wxPaintEvent& event);

	void Draw(wxDC& dc) const;

	void Resize(float max_len);

private:
	wxImage m_image;
	wxBitmap m_resized;
	
	DECLARE_EVENT_TABLE()

}; // ImagePanel

}

#endif // _EASYEDITOR_IMAGE_PANEL_H_