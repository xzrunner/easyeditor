#pragma once



namespace eanim
{
	class LibrarySymbolPage : public ee::ILibraryPage
	{
	public:
		LibrarySymbolPage(wxWindow* parent);

		virtual bool isHandleSymbol(ee::ISymbol* symbol) const;

	protected:
		virtual void onAddPress(wxCommandEvent& event);

	}; // LibrarySymbolPage
}

