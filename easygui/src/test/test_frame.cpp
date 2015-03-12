#include <easygui.h>

void main()
{
	egui::Frame* frame1 = new egui::Frame("test1");
	frame1->Show();

	egui::Frame* frame2 = new egui::Frame("test2");
	frame2->Show();

	egui::Dialog* dlg = new egui::Dialog(frame2, "dlg");
	dlg->ShowModal();
} 