#ifndef _ESHADER_LIBRARY_PANEL_H_
#define _ESHADER_LIBRARY_PANEL_H_

#include <drag2d.h>

namespace eshader
{

class LibraryPanel : public d2d::LibraryPanel
{
public:
	LibraryPanel(wxWindow* parent, bool is_2d);

}; // LibraryPanel

}

#endif // _ESHADER_LIBRARY_PANEL_H_