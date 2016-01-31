#pragma once



namespace eanim
{
	class LibrarySkeletonPage : public ee::ILibraryPage
	{
	public:
		LibrarySkeletonPage(wxWindow* parent);

		virtual bool isHandleSymbol(ee::ISymbol* symbol) const;

 	protected:
 		virtual void onAddPress(wxCommandEvent& event);

	}; // LibrarySkeletonPage
}

