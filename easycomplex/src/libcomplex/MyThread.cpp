#include "MyThread.h"

#include <drag2d.h>

DEFINE_EVENT_TYPE(wxEVT_MYTHREAD)
MyThread::MyThread() : wxThread(wxTHREAD_DETACHED)
{
}
void* MyThread::Entry()
{
// 	wxCommandEvent evt(wxEVT_MYTHREAD, GetId());
// 	//can be used to set some identifier for the data
// 	evt.SetInt(r); 
// 	//whatever data your thread calculated, to be returned to GUI
// 	evt.SetClientData(data); 
//	wxPostEvent(m_pParent, evt);

	const char* filepath = "png\\2208-tongyong-1.png";
	int w, h, c;
	unsigned int tex;
	for (int i = 0; i < 100; ++i) {
		d2d::ImageLoader::loadTexture(filepath, w, h, tex, c);
	}

	return 0;
}