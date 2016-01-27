#include "FrameDropTarget.h"
#include "Frame.h"

namespace d2d
{

bool FrameDropTarget::OnDropFiles(wxCoord x, wxCoord y, 
								  const wxArrayString& filenames)
{
	if (filenames.IsEmpty()) {
		return true;
	}

	wxString filename = filenames[0];
	m_frame->OpenFile(filename);

	return true;
}

}