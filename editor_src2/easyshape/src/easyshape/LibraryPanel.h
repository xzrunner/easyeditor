
#ifndef ESHAPE_LIBRARY_PANEL_H
#define ESHAPE_LIBRARY_PANEL_H

#include <drag2d.h>

namespace eshape
{
	class StagePanel;
	class ToolbarPanel;
	class LibraryShapePage;
	class LibraryImagePage;

	class LibraryPanel : public d2d::LibraryPanel
	{
	public:
		LibraryPanel(wxWindow* parent);

		//
		// ISerializable interface
		//
		virtual void loadFromTextFile(std::ifstream& fin);
		virtual void storeToTextFile(std::ofstream& fout) const;

		virtual void onPageChanged(wxBookCtrlEvent& event);

		d2d::ILibraryPage* getShapePage();
		d2d::ILibraryPage* getImagePage();

	private:
		LibraryShapePage* m_shapePage;
		LibraryImagePage* m_imagePage;

	}; // LibraryPanel
}

#endif // ESHAPE_LIBRARY_PANEL_H