
#ifndef EBUILDER_ACTORS_LIST_H
#define EBUILDER_ACTORS_LIST_H



namespace ebuilder
{
	class Layer;

	class ActorsList : public ee::LibraryList
	{
	public:
		ActorsList(wxWindow* parent);

		virtual void onListSelected(wxCommandEvent& event);

		void loadFromLayer(Layer* layer);

	}; // ActorsList
}

#endif // EBUILDER_ACTORS_LIST_H
