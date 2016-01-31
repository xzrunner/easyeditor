#pragma once



namespace coceditor
{
	class LibraryComplexPage;

	class LibraryPanel : public ee::LibraryPanel
	{
	public:
		LibraryPanel(wxWindow* parent);

		//
		//ISerializable interface
		//

		virtual void loadFromTextFile(std::ifstream& fin);
		virtual void storeToTextFile(std::ofstream& fout) const;

		//virtual void onPageChanged(wxBookCtrlEvent& event);

		ee::ILibraryPage* getImagePage();
		ee::ILibraryPage* getComplexPage();
		ee::ILibraryPage* getAnimPage();
		ee::ILibraryPage* get9PatchPage();

	private:
		ee::LibraryImagePage* m_imagePage;
		LibraryComplexPage* m_complexPage;
		ee::LibraryAnimPage* m_animPage;
		ee::Library9PatchPage* m_scale9Page;

	}; // LibraryPanel
}

