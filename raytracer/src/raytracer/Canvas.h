#ifndef _RT_CANVAS_H_
#define _RT_CANVAS_H_

#include <wx/wx.h>

#include "Thread.h"

namespace rt
{

class World;
class Frame;

class Canvas: public wxScrolledWindow
{
public:
   Canvas(Frame* frame);
   virtual ~Canvas(void);
    
   void SetImage(wxImage& image);
   wxImage GetImage(void);
   
   virtual void OnDraw(wxDC& dc);
   void renderStart(void);
   void renderPause(void);
   void renderResume(void);
   void OnRenderCompleted( wxCommandEvent& event );
   void OnTimerUpdate( wxTimerEvent& event );
   void OnNewPixel( wxCommandEvent& event );

protected:
   wxBitmap *m_image;
   World* m_world;

private:
	Frame* m_frame;

   Thread* thread;
   wxStopWatch* timer;
   long pixelsRendered;
   long pixelsToRender;
   wxTimer updateTimer;
   
   DECLARE_EVENT_TABLE()
};

}

#endif // _RT_CANVAS_H_