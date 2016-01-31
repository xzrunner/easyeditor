#pragma once



namespace coceditor
{
	class LibraryComplexList : public ee::LibraryList
	{
	public:
		LibraryComplexList(wxWindow* parent);

		virtual void onListDoubleClicked(wxCommandEvent& event);

	}; // LibraryComplexList 
}

