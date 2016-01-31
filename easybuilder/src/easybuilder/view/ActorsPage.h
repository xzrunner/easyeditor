
#ifndef EBUILDER_ACTORS_PAGE_H
#define EBUILDER_ACTORS_PAGE_H



namespace ebuilder
{
	class ActorsPage : public ee::ILibraryPage
	{
	public:
		ActorsPage(wxWindow* parent);

		//
		// ee::ILibraryPage interface
		//
		virtual bool isHandleSymbol(ee::ISymbol* symbol) const;

	protected:
		virtual void initLayout(bool draggable = true);

		virtual void onAddPress(wxCommandEvent& event);
		virtual void onDelPress(wxCommandEvent& event);

	protected:
		void initButtons(wxSizer* sizer);

	}; // ActorsPage
}

#endif // EBUILDER_ACTORS_PAGE_H
