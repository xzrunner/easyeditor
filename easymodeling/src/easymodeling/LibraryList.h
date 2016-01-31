
#ifndef EMODELING_LIBRARY_LIST_H
#define EMODELING_LIBRARY_LIST_H



namespace emodeling
{
	class LibraryList : public ee::LibraryList
	{
	public:
		LibraryList(ee::LibraryPage* page);

		virtual void OnListDoubleClicked(wxCommandEvent& event);

	}; // LibraryPage
}

#endif // EMODELING_LIBRARY_LIST_H