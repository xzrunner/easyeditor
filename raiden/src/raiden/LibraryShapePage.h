#ifndef RAIDEN_LIBRARY_SHAPE_PAGE_H
#define RAIDEN_LIBRARY_SHAPE_PAGE_H



namespace raiden
{
	class LibraryShapePage : public ee::ILibraryPage
	{
	public:
		LibraryShapePage(wxWindow* parent);

		virtual bool isHandleSymbol(ee::ISymbol* symbol) const;

	protected:
		virtual void initLayout(bool draggable = true);

		virtual void onAddPress(wxCommandEvent& event);

	private:
		void onNewBtnPress(wxCommandEvent& event);

	}; // LibraryShapePage
}

#endif // RAIDEN_LIBRARY_SHAPE_PAGE_H