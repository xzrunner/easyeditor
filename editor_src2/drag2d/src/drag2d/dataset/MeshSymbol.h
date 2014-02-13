#pragma once

#include <vector>

#include "ISymbol.h"
#include "Image.h"

#include "common/Vector.h"

namespace d2d
{
	class MeshSymbol : public ISymbol
	{
	public:
		MeshSymbol();
		virtual ~MeshSymbol();

		//
		// IObject interface
		//	
		virtual MeshSymbol* clone() const;

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
		virtual Rect getSize(const ISprite* sprite = NULL) const;

		const std::vector<Vector>& getVertices() const;

	private:
		void loadResources();

	private:
		Image* m_image;

		std::vector<Vector> m_vertices;
		std::vector<Vector> m_texCoords;

	}; // MeshSymbol
}

