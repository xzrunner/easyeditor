#include "gui_factory.h"

#ifdef EGUI_WX
#include "wx/FrameWX.h"
#include "wx/DialogWX.h"
#endif // EGUI_WX

namespace egui
{

Frame* create_frame(const std::string& title)
{
	Frame* frame = NULL;
#ifdef EGUI_WX
	frame = new FrameWX(title);
#endif // EGUI_WX
	return frame;
}

Dialog* create_dialog(Window* parent, const std::string& title)
{
	Dialog* dlg = NULL;
#ifdef EGUI_WX
	dlg = new DialogWX(parent, title);
#endif // EGUI_WX
	return dlg;
}

}