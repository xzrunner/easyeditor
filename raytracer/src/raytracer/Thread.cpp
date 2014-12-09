#include "Thread.h"
#include "Canvas.h"
#include "Pixel.h"

#include <raytracer.h>

namespace rt
{

DEFINE_EVENT_TYPE(wxEVT_RENDER)

void Thread::SetPixel(int x, int y, int red, int green, int blue)
{
   
   pixels.push_back(new Pixel(x, y, red, green, blue));
   
   if(timer->Time() - lastUpdateTime > 40)
      NotifyCanvas();
    
   TestDestroy();
}

void Thread::NotifyCanvas()
{
   lastUpdateTime = timer->Time();
   
   //copy rendered pixels into a new vector and reset
   std::vector<Pixel*> *pixelsUpdate = new std::vector<Pixel*>(pixels);
   pixels.clear();
   
   wxCommandEvent event(wxEVT_RENDER, ID_RENDER_NEWPIXEL);
   event.SetClientData(pixelsUpdate);
   canvas->GetEventHandler()->AddPendingEvent(event);
}

void Thread::OnExit()
{
   NotifyCanvas();
   
   wxCommandEvent event(wxEVT_RENDER, ID_RENDER_COMPLETED);
   
   canvas->GetEventHandler()->AddPendingEvent(event);
   
   canvas->GetParent()->GetEventHandler()->AddPendingEvent(event);
}

void *Thread::Entry()
{
   lastUpdateTime = 0;
   timer = new wxStopWatch();
   
   //world->render_scene(); //for bare bones ray tracer only
   world->GetCamera()->RenderScene(*world);

   return NULL;
}


}