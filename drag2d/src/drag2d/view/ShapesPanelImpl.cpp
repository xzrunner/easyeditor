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

bool ShapesPanelImpl::InsertShape(IShape* shape)
{
	return m_container->Insert(shape);
}

bool ShapesPanelImpl::RemoveShape(IShape* shape)
{
	return m_container->Remove(shape);
}

bool ShapesPanelImpl::ClearAllShapes()
{
	return m_container->Clear();
}

void ShapesPanelImpl::TraverseShapes(IVisitor& visitor, DataTraverseType type/* = e_allExisting*/) const
{
	m_container->Traverse(visitor, true);
}

} // d2d