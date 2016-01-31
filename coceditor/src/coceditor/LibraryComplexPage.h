#pragma once



namespace coceditor
{
	class LibraryComplexPage : public ee::ILibraryPage
	{
	public:
		LibraryComplexPage(wxWindow* parent);

		virtual bool isHandleSymbol(ee::ISymbol* symbol) const;

	protected:
		virtual void initLayout(bool draggable = true);

		virtual void onAddPress(wxCommandEvent& event);

	private:
		void onNewBtnPress(wxCommandEvent& event);

	}; // LibraryComplexPage
}

