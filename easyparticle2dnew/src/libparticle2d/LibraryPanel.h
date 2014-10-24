#ifndef _EASYPARTICLE2D_LIBRARY_PANEL_H_
#define _EASYPARTICLE2D_LIBRARY_PANEL_H_

#include <drag2d.h>

namespace anim { class LibraryPage; }
namespace ecomplex { class LibraryPage; }
namespace escale9 { class LibraryPage; }

namespace eparticle2d
{

class LibraryPanel : public d2d::LibraryPanel
{
public:
	LibraryPanel(wxWindow* parent);

}; // LibraryPanel

}

#endif // _EASYPARTICLE2D_LIBRARY_PANEL_H_