#pragma once

#include <drag2d.h>

namespace anim { class LibraryPage; }
namespace complex { class LibraryPage; }

namespace eparticle
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
	d2d::ILibraryPage* get9PatchPage() { return m_9patchPage; }

private:
	d2d::LibraryImagePage* m_imagePage;
	complex::LibraryPage* m_complexPage;
	anim::LibraryPage* m_animPage;
	d2d::Library9PatchPage* m_9patchPage;

}; // LibraryPanel

}

