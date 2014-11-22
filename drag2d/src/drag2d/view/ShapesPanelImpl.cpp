#include "ShapesPanelImpl.h"

#include "common/tools.h"

namespace d2d
{

ShapesPanelImpl::ShapesPanelImpl(wxWindow* wnd)
	: MultiShapesImpl(wnd)
{
}

ShapesPanelImpl::~ShapesPanelImpl()
{
	clearShapes();
}

void ShapesPanelImpl::traverseShapes(IVisitor& visitor, 
									 TraverseType type/* = e_allExisting*/) const
{
	std::vector<IShape*>::const_iterator itr = m_shapes.begin();
	for ( ; itr != m_shapes.end(); ++itr)
	{
		bool hasNext;
		visitor.visit(*itr, hasNext);
		if (!hasNext) break;
	}
}

void ShapesPanelImpl::removeShape(IShape* shape)
{
	std::vector<IShape*>::iterator itr = m_shapes.begin();
	for ( ; itr != m_shapes.end(); ++itr)
	{
		if (*itr == shape)
		{
			m_shapes.erase(itr);
			shape->Release();
			break;
		}
	}
}

void ShapesPanelImpl::insertShape(IShape* shape)
{
	m_shapes.push_back(shape);
	refresh();
}

void ShapesPanelImpl::clearShapes()
{
	for_each(m_shapes.begin(), m_shapes.end(), DeletePointerFunctor<IShape>());
	m_shapes.clear();
}

} // d2d