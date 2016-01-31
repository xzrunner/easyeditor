
#ifndef EBUILDER_LIST_OBSERVER_H
#define EBUILDER_LIST_OBSERVER_H



namespace ebuilder
{
	class Behavior;
	class BehaviorPanel;

	class ListObserver
	{
	public:
		ListObserver(ee::LibraryList* list);
		
		void registerListener(Behavior* lsn);
		void registerListener(BehaviorPanel* lsn);

		void removeListener(Behavior* lsn);
		void clearUIListeners();

		void updateListeners();

	private:
		ee::LibraryList* m_list;

		std::vector<Behavior*> m_listeners;
		std::vector<BehaviorPanel*> m_uiListeners;

	}; // ListObserver
}

#endif // EBUILDER_LIST_OBSERVER_H
