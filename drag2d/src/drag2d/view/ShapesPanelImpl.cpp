#include "ShapesPanelImpl.h"
#include "IDataContainer.h"
#include "ShapesContainer.h"

#include "common/tools.h"

namespace d2d
{

ShapesPanelImpl::ShapesPanelImpl(wxWindow* parent)
	: MultiShapesImpl(parent)
{
	m_container = new ShapesContainer();
}

ShapesPanelImpl::ShapesPanelImpl(wxWindow* parent, IDataContainer* container)
	: MultiShapesImpl(parent)
{
	m_container = container;
	m_container->Retain();
}

ShapesPanelImpl::~ShapesPanelImpl()
{
	m_container->Release();
}

void ShapesPanelImpl::traverseShapes(IVisitor& visitor, TraverseType type/* = e_allExisting*/) const
{
	m_container->traverse(visitor);
}

void ShapesPanelImpl::removeShape(IShape* shape)
{
	m_container->remove(shape);
}

void ShapesPanelImpl::insertShape(IShape* shape)
{
	m_container->insert(shape);
	refresh();
}

void ShapesPanelImpl::clearShapes()
{
	m_container->clear();
}

} // d2d