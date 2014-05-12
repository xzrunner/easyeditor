#pragma once

#include "MultiShapesImpl.h"

namespace d2d
{
	class IShape;

	class ShapesPanelImpl : public MultiShapesImpl
	{
	public:
		ShapesPanelImpl(wxWindow* wnd);
		virtual ~ShapesPanelImpl();

		virtual void traverseShapes(IVisitor& visitor, 
			TraverseType type = e_allExisting) const;
		virtual void removeShape(IShape* shape);
		virtual void insertShape(IShape* shape);
		virtual void clearShapes();

	private:
		std::vector<IShape*> m_shapes;

	}; // ShapesPanelImpl
}

