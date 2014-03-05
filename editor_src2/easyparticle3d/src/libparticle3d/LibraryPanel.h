#ifndef _EASYPARTICLE3D_LIBRARY_PANEL_H_
#define _EASYPARTICLE3D_LIBRARY_PANEL_H_

#include <drag2d.h>

namespace anim { class LibraryPage; }
namespace complex { class LibraryPage; }

namespace eparticle3d
{

class LibraryPanel : public d2d::LibraryPanel
{
public:
	LibraryPanel(wxWindow* parent);

	//
	//ISerializable interface
	//
	virtual void loadFromTextFile(std::ifstream& fin);
	virtual void storeToTextFile(std::ofstream& fout) const;

	d2d::ILibraryPage* getImagePage() { return m_imagePage; }
	d2d::ILibraryPage* getComplexPage();
	d2d::ILibraryPage* getAnimPage();
	d2d::ILibraryPage* getScale9Page() { return m_scale9Page; }

private:
	d2d::LibraryImagePage* m_imagePage;
	complex::LibraryPage* m_complexPage;
	anim::LibraryPage* m_animPage;
	d2d::LibraryScale9Page* m_scale9Page;

}; // LibraryPanel

}

#endif // _EASYPARTICLE3D_LIBRARY_PANEL_H_