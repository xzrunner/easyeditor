#pragma once

#include <drag2d.h>

#include "dataset/WholeSkeleton.h"

namespace eanim
{
	class LibraryPanel;
	class StagePanel;
	class WholeSkeleton;

	class LibrarySkeletonList : public d2d::LibraryList
	{
	public:
		LibrarySkeletonList(wxWindow* parent);
	
		//
		// d2d::ISerializable interface
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

