#pragma once

#include <drag2d.h>

namespace anim { class LibraryPage; }
namespace emesh { class LibraryPage; }

namespace ecomplex
{
	class LibraryPage;

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
		d2d::ILibraryPage* getMeshPage();
		d2d::ILibraryPage* getFontPage() { return m_fontPage; }
		d2d::ILibraryPage* getScriptsPage() { return m_scriptsPage; }

	private:
		d2d::LibraryImagePage* m_imagePage;
		LibraryPage* m_complexPage;
		anim::LibraryPage* m_animPage;
		emesh::LibraryPage* m_meshPage;
		d2d::LibraryFontBlankPage* m_fontPage;
		d2d::LibraryScriptsPage* m_scriptsPage;

	}; // LibraryPanel
}

