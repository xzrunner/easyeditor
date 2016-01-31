
#ifndef EBUILDER_SCENES_PAGE_H
#define EBUILDER_SCENES_PAGE_H



namespace ebuilder
{
	class ListObserver;
	class Behavior;

	class ScenesPage : public ee::ILibraryPage
	{
	public:
		ScenesPage(wxWindow* parent);
		virtual ~ScenesPage();

		virtual bool isHandleSymbol(ee::ISymbol* symbol) const;

		ListObserver* getListObserver() {
			return m_observer;
		}

		void insert();

		void enableDelBtn();

	protected:
		virtual void initLayout(bool draggable = true);

		virtual void onAddPress(wxCommandEvent& event);
		virtual void onDelPress(wxCommandEvent& event);

	private:
		ListObserver* m_observer;

	}; // ScenesPage
}

#endif // EBUILDER_SCENES_PAGE_H
