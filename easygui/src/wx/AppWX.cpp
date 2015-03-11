#include <wx/wx.h>

void main();

class AppImpl : public wxApp
{
public:
	virtual bool OnInit();

}; // AppImpl

IMPLEMENT_APP(AppImpl)

bool AppImpl::OnInit()
{
	main();
	return true;
}
