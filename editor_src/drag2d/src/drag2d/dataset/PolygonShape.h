#pragma once

#include "ChainShape.h"

#include <wx/wx.h>

namespace d2d
{
	class ImageSymbol;

	class PolygonShape : public ChainShape
	{
	public:
		enum FillingType
		{
			e_Color = 0,
			e_Texture,
		};

	public:
		PolygonShape(const PolygonShape& polygon);
		PolygonShape(const std::vector<Vector>& vertices);
		virtual ~PolygonShape();
		
		//
		// IObject interface
		//
		virtual PolygonShape* clone();

		//
		// IShape interface
		//
		virtual void draw(const Colorf& color = Colorf(0, 0, 0)) const;

		//
		// ChainShape interface
		//
		virtual void refresh();

	public:
		void buildFillingTris();
		void buildFillingTris(const std::vector<Vector>& segments);

	private:
		void getBoundingRegion(const std::vector<Vector>& bounding, float& left, float& right, float& low, float& up);
		void getTextureBoundarySegments(float left, float right, float low, float up,
			std::vector<Vector>& segments);
		void computeTextureCoords(float left, float low);

	public:
		int m_fillingType;

		Colorf m_fillingColor;
		ImageSymbol* m_fillingTexture;

		std::vector<Vector> m_fillingVertices;
		std::vector<Vector> m_fillingTexCoords;

	}; // PolygonShape
}

