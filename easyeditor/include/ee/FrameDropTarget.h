#ifndef _EASYEDITOR_FRAME_DROP_TARGET_H_
#define _EASYEDITOR_FRAME_DROP_TARGET_H_

#include <wx/dnd.h>

namespace ee
{

class Frame;

class FrameDropTarget : public wxFileDropTarget
{
public:
	FrameDropTarget(Frame* frame) 
		: m_frame(frame) {}

	virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames);

private:
	Frame* m_frame;

}; // FrameDropTarget

}

#endif // _EASYEDITOR_FRAME_DROP_TARGET_H_