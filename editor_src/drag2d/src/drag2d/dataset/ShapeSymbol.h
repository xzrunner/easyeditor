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
		// IObject interface
		//	
		virtual ShapeSymbol* clone();

		//
		// ISerializable interface
		//
		virtual void loadFromTextFile(std::ifstream& fin);
		virtual void storeToTextFile(std::ofstream& fout) const;

		//
		// ISymbol interface
		//
		virtual void reloadTexture() const;
		virtual void draw(const ISprite* sprite = NULL) const;
		virtual float getWidth(const ISprite* sprite = NULL) const;
		virtual float getHeight(const ISprite* sprite = NULL) const;

	private:
		void loadResources();

		void clear();

	public:
		std::vector<IShape*> shapes;

	}; // ShapeSymbol
}

