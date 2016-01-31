#ifndef FORMATION_LIBRARY_IMAGE_PAGE_H
#define FORMATION_LIBRARY_IMAGE_PAGE_H



namespace formation
{
	class LibraryImagePage : public ee::LibraryImagePage
	{
	public:
		LibraryImagePage(wxWindow* parent);

	protected:
		virtual void onAddPress(wxCommandEvent& event);
		virtual void onDelPress(wxCommandEvent& event);

	}; // LibraryImagePage
}

#endif // FORMATION_LIBRARY_IMAGE_PAGE_H