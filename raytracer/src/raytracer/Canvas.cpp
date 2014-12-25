#include "Canvas.h"
#include "Frame.h"
#include "Pixel.h"
#include "bg.xpm"

#include <raytracer.h>

#include <wx/dcbuffer.h>

namespace rt
{

BEGIN_EVENT_TABLE(Canvas, wxScrolledWindow )
   EVT_COMMAND(ID_RENDER_NEWPIXEL, wxEVT_RENDER, Canvas::OnNewPixel)
   EVT_COMMAND(ID_RENDER_COMPLETED, wxEVT_RENDER, Canvas::OnRenderCompleted)
   EVT_TIMER(ID_RENDER_UPDATE, Canvas::OnTimerUpdate)
END_EVENT_TABLE()

Canvas::Canvas(Frame* frame)
	: wxScrolledWindow(frame), m_image(NULL), m_world(NULL), thread(NULL), timer(NULL), updateTimer(this, ID_RENDER_UPDATE)
	, m_frame(frame)
{
	SetOwnBackgroundColour(wxColour(143,144,150));
}

Canvas::~Canvas(void)
{
	renderPause();

	if(m_image != NULL)
		delete m_image;

	if(thread != NULL)
		thread->Delete();

	if(m_world != NULL)
		delete m_world;

	if(timer != NULL)
		delete timer;
}

void Canvas::SetImage(wxImage& image)
{
	if(m_image != NULL)
		delete m_image;

	m_image = new wxBitmap(image);

	SetScrollbars(10, 10, (int)(m_image->GetWidth()  / 10.0f),
		(int)(m_image->GetHeight() / 10.0f), 0, 0, true);

	Refresh();
}

wxImage Canvas::GetImage(void)
{
	if(m_image != NULL)
		return m_image->ConvertToImage();

	return wxImage();
}

void Canvas::OnDraw(wxDC& dc)
{
	if(m_image != NULL && m_image->IsOk())
		wxBufferedDC bdc(&dc, *m_image);
}

void Canvas::OnRenderCompleted( wxCommandEvent& event )
{
	thread = NULL;

	if(m_world != NULL)
	{
		delete m_world;
		m_world = NULL;
	}

	if(timer != NULL)
	{
		long interval = timer->Time();

		wxTimeSpan timeElapsed(0, 0, 0, interval);
		wxString timeString = timeElapsed.Format(wxT("Elapsed Time: %H:%M:%S"));
		m_frame->SetStatusText( timeString, 1);

		delete timer;
		timer = NULL;
	}
}

void Canvas::OnNewPixel( wxCommandEvent& event )
{
	//set up double buffered device context
	wxClientDC cdc(this);
	DoPrepareDC(cdc);
	wxBufferedDC bufferedDC(&cdc, *m_image);

	//iterate over all pixels in the event
	std::vector<Pixel*> *pixelsUpdate =
		(std::vector<Pixel*> *)event.GetClientData();

	for(std::vector<Pixel*>::iterator itr = pixelsUpdate->begin();
		itr != pixelsUpdate->end(); itr++)
	{
		Pixel* pixel = *itr;

		wxPen pen(wxColour(pixel->red, pixel->green, pixel->blue));
		bufferedDC.SetPen(pen);
		bufferedDC.DrawPoint(pixel->x, pixel->y);

		pixelsRendered++;
		delete pixel;
	}

	pixelsUpdate->clear();
	delete pixelsUpdate;
}

void Canvas::renderPause(void)
{
	if(thread != NULL)
		thread->Pause();

	updateTimer.Stop();

	if(timer != NULL)
		timer->Pause();
}

void Canvas::renderResume(void)
{
	if(thread != NULL)
		thread->Resume();

	updateTimer.Start();

	if(timer != NULL)
		timer->Resume();
}

void Canvas::OnTimerUpdate( wxTimerEvent& event )
{
	if(timer == NULL)
		return;

	//percent
	float completed = (float)pixelsRendered / (float)pixelsToRender;

	wxString progressString = wxString::Format(wxT("Rendering...%d%%"),
		(int)(completed*100));
	m_frame->SetStatusText( progressString , 0);

	//time elapsed
	long interval = timer->Time();

	wxTimeSpan timeElapsed(0, 0, 0, interval);

	//time remaining
	float remaining = 1.0f - completed;
	long msecRemain = (long)
		(((double)interval / (completed*100)) * 100 * remaining);

	wxTimeSpan timeRemaining(0, 0, 0, msecRemain);

	wxString timeRemainString = timeRemaining.Format(wxT(" / ETA: %H:%M:%S"));
	wxString timeString = timeElapsed.Format(wxT("Elapsed Time: %H:%M:%S"));

	//only display ETA if something has been completed
	if(msecRemain >= 0)
		m_frame->SetStatusText( timeString + timeRemainString, 1);
	else
		m_frame->SetStatusText( timeString, 1);
}

void Canvas::renderStart(void)
{
	m_world = new World();

	m_frame->SetStatusText( wxT( "Building world..." ) );

	m_world->Build();

	m_frame->SetStatusText( wxT( "Rendering..." ) );

	int w = m_world->GetViewPlane().GetWidth(),
		h = m_world->GetViewPlane().GetHeight();

	pixelsRendered = 0;
	pixelsToRender = w * h;

	//set the background
	wxBitmap bitmap(w, h, -1);
	wxMemoryDC dc;
	dc.SelectObject(bitmap);
	dc.SetBackground(*wxGREY_BRUSH);
	dc.Clear();

	wxBitmap tile(bg_xpm);

	for(int x = 0; x < w; x += 16)
	{
		for(int y = 0; y < h; y += 16)
			dc.DrawBitmap(tile, x, y, FALSE);
	}

	dc.SelectObject(wxNullBitmap);

	wxImage temp = bitmap.ConvertToImage();
	SetImage(temp);

	updateTimer.Start(250);

	//start timer
	timer = new wxStopWatch();

	thread = new Thread(this, m_world);
	thread->Create();
	m_world->SetRenderOutput(thread);
	thread->SetPriority(20);
	thread->Run();
}

}