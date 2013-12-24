#pragma once

#include "ChainShape.h"

namespace d2d
{
	class CosineShape : public ChainShape
	{
	public:
		CosineShape(const CosineShape& curve);
		CosineShape(const std::vector<Vector>& vertices);
		virtual ~CosineShape();

		//
		// IObject interface
		//
		virtual CosineShape* clone();

		//
		// IShape interface
		//
		virtual void draw(const Colorf& color = Colorf(0, 0, 0)) const;

		//
		// ChainShape interface
		//
		virtual void insertVertices(size_t index, const Vector& pos);
		virtual void removeVertices(const Vector& pos);
		virtual void changeVertices(const Vector& from, const Vector& to);
		virtual void refresh();

		const std::vector<Vector>& getMidPoints() const { return m_midPoints; }
		void setMidPoints();

	private:
		static const float SAMPLING_WIDTH;

	private:
		std::vector<Vector> m_midPoints;

	}; // CosineShape
}

