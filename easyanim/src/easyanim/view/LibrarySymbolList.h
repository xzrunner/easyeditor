#pragma once



namespace eanim
{
	class LibrarySymbolList : public ee::LibraryList
	{
	public:
		LibrarySymbolList(wxWindow* parent);

		//
		// ee::ISerializable interface
		//
		virtual void loadFromTextFile(std::ifstream& fin);
		virtual void storeToTextFile(std::ofstream& fout) const;

	protected:
		virtual void onListDoubleClicked(wxCommandEvent& event);

	}; // LibrarySymbolList
}

