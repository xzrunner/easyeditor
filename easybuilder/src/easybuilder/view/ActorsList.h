
#ifndef EBUILDER_ACTORS_LIST_H
#define EBUILDER_ACTORS_LIST_H

#include <drag2d.h>

namespace ebuilder
{
	class Layer;

	class ActorsList : public d2d::LibraryList
	{
	public:
		ActorsList(wxWindow* parent);

		virtual void onListSelected(wxCommandEvent& event);

		void loadFromLayer(Layer* layer);

	}; // ActorsList
}

#endif // EBUILDER_ACTORS_LIST_H
