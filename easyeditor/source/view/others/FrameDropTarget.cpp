#include "FrameDropTarget.h"
#include "Frame.h"

namespace ee
{

bool FrameDropTarget::OnDropFiles(wxCoord x, wxCoord y, 
								  const wxArrayString& filenames)
{
	if (filenames.IsEmpty()) {
		return true;
	}

	std::string filename = filenames[0];
	m_frame->OpenFile(filename);

	return true;
}

}