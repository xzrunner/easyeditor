#ifndef _RT_THREAD_H_
#define _RT_THREAD_H_

#include <wx/wx.h>
#include <vector>

#include <raytracer.h>

namespace rt
{

DECLARE_EVENT_TYPE(wxEVT_RENDER, -1)
#define ID_RENDER_COMPLETED 100
#define ID_RENDER_NEWPIXEL  101
#define ID_RENDER_UPDATE    102

class Canvas;
class World;
class Pixel;

class Thread : public wxThread, public RenderOutput
{
public:
   Thread(Canvas* c, World* w) : wxThread(), world(w), canvas(c){}
   virtual void *Entry();
   virtual void OnExit();
   virtual void SetPixel(int x, int y, int red, int green, int blue);

private:
   void NotifyCanvas();
   
   World* world;
   Canvas* canvas;
   
   std::vector<Pixel*> pixels;
   wxStopWatch* timer;
   long lastUpdateTime;
};

}

#endif // _RT_THREAD_H_