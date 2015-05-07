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

void ShapesPanelImpl::traverseShapes(IVisitor& visitor, DataTraverseType type/* = e_allExisting*/) const
{
	m_container->Traverse(visitor, true);
}

void ShapesPanelImpl::removeShape(IShape* shape)
{
	m_container->Remove(shape);
}

void ShapesPanelImpl::insertShape(IShape* shape)
{
	m_container->Insert(shape);
	refresh();
}

void ShapesPanelImpl::clearShapes()
{
	m_container->Clear();
}

} // d2d