#ifndef RAIDEN_LIBRARY_SHAPE_LIST_H
#define RAIDEN_LIBRARY_SHAPE_LIST_H



namespace raiden
{
	class LibraryShapeList : public ee::LibraryList
	{
	public:
		LibraryShapeList(wxWindow* parent);

		virtual void onListDoubleClicked(wxCommandEvent& event);

	}; // LibraryShapeList
}

#endif // RAIDEN_LIBRARY_SHAPE_LIST_H