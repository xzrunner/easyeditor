#pragma once



namespace ecomplex
{
	class LibraryPanel : public ee::LibraryPanel
	{
	public:
		LibraryPanel(wxWindow* parent);

		//
		//ISerializable interface
		//

		virtual void loadFromTextFile(std::ifstream& fin);
		virtual void storeToTextFile(std::ofstream& fout) const;

		ee::ILibraryPage* getImagePage() { return m_imagePage; }
		ee::ILibraryPage* getComplexPage() { return m_complexPage; }
		ee::ILibraryPage* getAnimPage() { return m_animPage; }
		ee::ILibraryPage* get9PatchPage() { return m_scale9Page; }
		ee::ILibraryPage* getFontPage() { return m_fontPage; }

	private:
		ee::LibraryImagePage* m_imagePage;
		ee::LibraryComplexPage* m_complexPage;
		ee::LibraryAnimPage* m_animPage;
		ee::Library9PatchPage* m_scale9Page;
		ee::LibraryFontBlankPage* m_fontPage;

	}; // LibraryPanel
}

