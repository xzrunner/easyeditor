#include <easygui.h>

void main()
{
	egui::Frame* frame = egui::create_frame("test1");
 	frame->Show();
 
 	frame = egui::create_frame("test2");
 	frame->Show();

	egui::Dialog* dlg = egui::create_dialog(frame, "dlg2");
	dlg->ShowModal();
}