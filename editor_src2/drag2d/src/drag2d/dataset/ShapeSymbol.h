#pragma once

#include <vector>

#include "ISymbol.h"

namespace d2d
{
	class IShape;

	class ShapeSymbol : public ISymbol
	{
	public:
		ShapeSymbol();
		ShapeSymbol(const ShapeSymbol& symbol);
		virtual ~ShapeSymbol();

		//
		// ICloneable interface
		//	
		virtual ShapeSymbol* clone() const;

		//
		// ISerializable interface
		//
		virtual void loadFromTextFile(std::ifstream& fin);
		virtual void storeToTextFile(std::ofstream& fout) const;

		//
		// ISymbol interface
		//
		virtual void reloadTexture() const;
		virtual void draw(const Colorf& mul = Colorf(1, 1, 1, 1), 
			const Colorf& add = Colorf(0, 0, 0, 0),
			const ISprite* sprite = NULL) const;
		virtual Rect getSize(const ISprite* sprite = NULL) const;

	private:
		void loadResources();

		void clear();

	public:
		std::vector<IShape*> shapes;

	}; // ShapeSymbol
}

