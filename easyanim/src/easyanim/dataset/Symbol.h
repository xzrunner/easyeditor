#pragma once

#include "WholeSkeleton.h"



namespace eanim
{
	class Mesh;
	class PartSkeleton;

	class Symbol : public ee::ImageSymbol
	{
	public:
		Symbol();
		Symbol(const wxString& filepath);
		virtual ~Symbol();
		
		////
		//// ee::ISerializable interface
		////
		//virtual void loadFromTextFile(std::ifstream& fin);
		//virtual void storeToTextFile(std::ofstream& fout) const;

// 		void loadFromModifiedData();
		void replaceBySelectedPixels(const ee::RawPixels::PixelBuf& selected);

		ee::RawPixels* getRawPixels();

		Mesh* getMesh();
		Mesh* createMesh();

		PartSkeleton* getSkeleton();
		PartSkeleton* createSkeleton();

		WholeSkeleton::Body* getSkeletonBody();
		WholeSkeleton::Body* createSkeletonBody();

	private:
		void clear();

	private:
		ee::RawPixels* m_pixelData;

		Mesh* m_mesh;
		PartSkeleton* m_skeleton;
		WholeSkeleton::Body* m_skeletonBody;

	}; // Symbol
}

