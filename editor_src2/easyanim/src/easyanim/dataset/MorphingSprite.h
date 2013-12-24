#pragma once

#include "Sprite.h"

namespace eanim
{
	class Symbol;
	class Mesh;
	class PartSkeleton;

	class MorphingSprite : public Sprite
	{
	public:
		MorphingSprite();
		MorphingSprite(Symbol* symbol, const d2d::Vector& pos, bool isCopyMesh);
		MorphingSprite(const MorphingSprite& sprite);
		virtual ~MorphingSprite();

		//
		// d2d::ICloneable interface
		//
		virtual MorphingSprite* clone();

		virtual void loadFromTextFile(std::ifstream& fin);
		virtual void storeToTextFile(std::ofstream& fout) const;

		virtual Symbol* getSymbol() { return m_symbol; }
		virtual Mesh* getMesh() { return m_mesh; }
		virtual PartSkeleton* getPartSkeleton() { return m_skeleton; }
		virtual WholeSkeleton* getWholeSkeleton() { return NULL; }

		virtual void reloadTexture();

	private:
		void clear();

		void initBounding(Symbol* symbol);

	private:
		Symbol* m_symbol;

		Mesh* m_mesh;
		PartSkeleton* m_skeleton;

	}; // MorphingSprite
}

