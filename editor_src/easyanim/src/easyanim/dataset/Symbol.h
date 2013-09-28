#pragma once

#include "WholeSkeleton.h"

#include <drag2d.h>

namespace eanim
{
	class Mesh;
	class PartSkeleton;

	class Symbol : public d2d::ImageSymbol
	{
	public:
		Symbol();
		Symbol(const wxString& filepath);
		virtual ~Symbol();
		
		////
		//// d2d::ISerializable interface
		////
		//virtual void loadFromTextFile(std::ifstream& fin);
		//virtual void storeToTextFile(std::ofstream& fout) const;

// 		void loadFromModifiedData();
		void replaceBySelectedPixels(const d2d::RawPixels::PixelBuf& selected);

		d2d::RawPixels* getRawPixels();

		Mesh* getMesh();
		Mesh* createMesh();

		PartSkeleton* getSkeleton();
		PartSkeleton* createSkeleton();

		WholeSkeleton::Body* getSkeletonBody();
		WholeSkeleton::Body* createSkeletonBody();

	private:
		void clear();

	private:
		d2d::RawPixels* m_pixelData;

		Mesh* m_mesh;
		PartSkeleton* m_skeleton;
		WholeSkeleton::Body* m_skeletonBody;

	}; // Symbol
}

