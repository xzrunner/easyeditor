#pragma once

#include <vector>

#include "IShape.h"

#include "common/Rect.h"

namespace d2d
{
	class ChainShape : public IShape
	{
	public:
		ChainShape();
		ChainShape(const ChainShape& chain);
		ChainShape(const std::vector<Vector>& vertices, bool isLoop);

		//
		// IObject interface
		//
		virtual ChainShape* clone() const;

		//
		// ISerializable interface
		//
		virtual void loadFromTextFile(std::ifstream& fin);
		virtual void storeToTextFile(std::ofstream& fout) const;

		//
		// IShape interface
		//
		virtual bool isContain(const Vector& pos) const;
		virtual bool isIntersect(const Rect& rect) const;
		virtual const Rect& getRect() const { return m_rect; }
		virtual void draw(const Colorf& color = Colorf(0, 0, 0)) const;

		virtual void insertVertices(size_t index, const Vector& pos);
		// todo: too slow! 
		// cache index or pointer? how to avoid iterator's invalid
		// maybe use list instead of array, but others' effective? 
 		virtual void removeVertices(const Vector& pos);
 		virtual void changeVertices(const Vector& from, const Vector& to);
		virtual void setVertices(const std::vector<Vector>& vertices);
		virtual void refresh() {}

		void setClosed(bool isClose) { m_isLoop = isClose; }
		bool isClosed() const { return m_isLoop; }

		const std::vector<Vector>& getVertices() const { return m_vertices; }

	private:
		void initBounding();

	protected:
		std::vector<Vector> m_vertices;
		bool m_isLoop;

		Rect m_rect;

	}; // ChainShape
}

