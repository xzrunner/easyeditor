#pragma once



namespace eanim
{
	class Symbol;
	class Mesh;
	class PartSkeleton;
	class WholeSkeleton;

	class Sprite : public ee::ImageSprite
	{
	public:

		virtual ~Sprite() {}

		//
		// ee::ICloneable interface
		//
		virtual Sprite* clone() const { return NULL; }

		virtual void loadFromTextFile(std::ifstream& fin) = 0;
		virtual void storeToTextFile(std::ofstream& fout) const = 0;

 		virtual Symbol* getSymbol() = 0;
 		virtual Mesh* getMesh() = 0;
 		virtual PartSkeleton* getPartSkeleton() = 0;
		virtual WholeSkeleton* getWholeSkeleton() = 0;

		virtual void reloadTexture() = 0;

	}; // Sprite
}

