#pragma once



#include "dataset/WholeSkeleton.h"

namespace eanim
{
	class LibraryPanel;
	class StagePanel;
	class WholeSkeleton;

	class LibrarySkeletonList : public ee::LibraryList
	{
	public:
		LibrarySkeletonList(wxWindow* parent);
	
		//
		// ee::ISerializable interface
		//
		virtual void loadFromTextFile(std::ifstream& fin);
		virtual void storeToTextFile(std::ofstream& fout) const;

	protected:
		void onListDoubleClicked(wxCommandEvent& event);

	private:
		void loadDataFromSymbolList(std::vector<WholeSkeleton::Node*>& nodes,
			std::vector<WholeSkeleton::Body*>& bodies) const;

	}; // LibrarySkeletonList
}

