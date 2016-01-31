#pragma once

#include "Sprite.h"

namespace eanim
{
	class WholeSkeleton;

	class SkeletalSprite : public Sprite
	{
	public:
		SkeletalSprite();
		SkeletalSprite(WholeSkeleton* skeleton);
		SkeletalSprite(const SkeletalSprite& sprite);
		virtual ~SkeletalSprite();

		//
		// ee::ICloneable interface
		//
		virtual SkeletalSprite* clone() const;

		void loadFromTextFile(std::ifstream& fin);
		void storeToTextFile(std::ofstream& fout) const;

		virtual Symbol* getSymbol() { return NULL; }
		virtual Mesh* getMesh() { return NULL; }
		virtual PartSkeleton* getPartSkeleton() { return NULL; }
		virtual WholeSkeleton* getWholeSkeleton() { return m_skeleton; }

		virtual void reloadTexture();

	private:
		void initBounding();

	private:
		class ReloadTextureVisitor : public ee::IVisitor
		{
		public:
			virtual void visit(ee::ICloneable* object, bool& bFetchNext);
		}; // ReloadTextureVisitor

	private:
		WholeSkeleton* m_src;
		WholeSkeleton* m_skeleton;

	}; // SkeletalSprite
}

