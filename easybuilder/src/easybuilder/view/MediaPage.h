
#ifndef EBUILDER_MEDIA_PAGE_H
#define EBUILDER_MEDIA_PAGE_H



namespace ebuilder
{
	class ListObserver;
	class Behavior;

	class MediaPage : public ee::ILibraryPage
	{
	public:
		MediaPage(wxWindow* parent);
		virtual ~MediaPage();

		virtual bool isHandleSymbol(ee::ISymbol* symbol) const;

		ListObserver* getListObserver() {
			return m_observer;
		}

	protected:
		virtual void onAddPress(wxCommandEvent& event);
		virtual void onDelPress(wxCommandEvent& event);

	private:
		ListObserver* m_observer;

	}; // MediaPage
}

#endif // EBUILDER_MEDIA_PAGE_H
