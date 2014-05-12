#pragma once

#include <drag2d.h>

namespace coceditor
{
	class LibraryComplexPage;

	class LibraryPanel : public d2d::LibraryPanel
	{
	public:
		LibraryPanel(wxWindow* parent);

		//
		//ISerializable interface
		//

		virtual void loadFromTextFile(std::ifstream& fin);
		virtual void storeToTextFile(std::ofstream& fout) const;

		//virtual void onPageChanged(wxBookCtrlEvent& event);

		d2d::ILibraryPage* getImagePage();
		d2d::ILibraryPage* getComplexPage();
		d2d::ILibraryPage* getAnimPage();
		d2d::ILibraryPage* get9PatchPage();

	private:
		d2d::LibraryImagePage* m_imagePage;
		LibraryComplexPage* m_complexPage;
		d2d::LibraryAnimPage* m_animPage;
		d2d::Library9PatchPage* m_scale9Page;

	}; // LibraryPanel
}

