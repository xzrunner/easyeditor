#pragma once

#include <vector>

#include "ISymbol.h"

namespace d2d
{
	class IShape;

	class EShapeSymbol : public ISymbol
	{
	public:
		EShapeSymbol();
		EShapeSymbol(const EShapeSymbol& symbol);
		virtual ~EShapeSymbol();

		//
		// IObject interface
		//	
		virtual EShapeSymbol* clone() const;

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

		//
		// ListItem interface
		//
		virtual void refresh();

	private:
		void loadResources();

		void loadThumbnail();

		void clear();

	private:
		static const float SCALE;

	public:
		std::vector<IShape*> shapes;

	}; // EShapeSymbol
}

