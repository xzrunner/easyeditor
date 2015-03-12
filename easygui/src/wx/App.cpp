#ifdef EGUI_WX

#include <wx/wx.h>

void main();

class App : public wxApp
{
public:
	virtual bool OnInit();

}; // App

IMPLEMENT_APP(App)

bool App::OnInit()
{
	main();
	return true;
}

#endif // EGUI_WX