
#ifndef EBUILDER_BEHAVIORS_PAGE_H
#define EBUILDER_BEHAVIORS_PAGE_H



namespace ebuilder
{
	class BehaviorsPage : public ee::ILibraryPage
	{
	public:
		BehaviorsPage(wxWindow* parent);

		virtual bool isHandleSymbol(ee::ISymbol* symbol) const;

		virtual void clear();

	protected:
		virtual void initLayout(bool draggable = true);

		virtual void onAddPress(wxCommandEvent& event);

	private:
		void initBehaviors();

	}; // BehaviorsPage
}

#endif // EBUILDER_BEHAVIORS_PAGE_H
