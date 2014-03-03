
#include "StagePanel.h"
#include "StageCanvas.h"
#include "LibraryItem.h"
#include "ToolBarPanel.h"
#include "Context.h"

using namespace eshape;

StagePanel::StagePanel(wxWindow* parent,
					   wxTopLevelWindow* frame)
	: EditPanel(parent, frame)
	, MultiShapesImpl(parent)
{
	m_canvas = new StageCanvas(this);
	m_item = NULL;
}

void StagePanel::clear()
{
	changeCurrItem(NULL);
	Context::Instance()->toolbar->changeCurrItem(NULL);

	d2d::EditPanel::clear();
	clearShapes();
}

void StagePanel::traverseShapes(d2d::IVisitor& visitor, 
								d2d::TraverseType type/* = d2d::e_allExisting*/) const
{
	if (!m_item || !m_item->getUserData())
		return;

	std::vector<d2d::IShape*>* shapes 
		= static_cast<std::vector<d2d::IShape*>*>(m_item->getUserData());
	std::vector<d2d::IShape*>::const_iterator itr = shapes->begin();
	for ( ; itr != shapes->end(); ++itr)
	{
		bool hasNext;
		visitor.visit(*itr, hasNext);
		if (!hasNext) break;
	}
}

void StagePanel::removeShape(d2d::IShape* shape)
{
	if (!m_item || !m_item->getUserData())
		return;

	std::vector<d2d::IShape*>* shapes 
		= static_cast<std::vector<d2d::IShape*>*>(m_item->getUserData());
	std::vector<d2d::IShape*>::iterator itr = shapes->begin();
	for ( ; itr != shapes->end(); ++itr)
	{
		if (*itr == shape)
		{
			shapes->erase(itr);
			shape->release();
			break;
		}
	}
}

void StagePanel::insertShape(d2d::IShape* shape)
{
	if (!m_item || !m_item->getUserData())
		return;

	std::vector<d2d::IShape*>* shapes 
		= static_cast<std::vector<d2d::IShape*>*>(m_item->getUserData());
//	static_cast<d2d::ChainShape*>(shape)->setClosed(Context::Instance()->isClosed);
	shapes->push_back(shape);
}

void StagePanel::clearShapes()
{
	if (!m_item || !m_item->getUserData())
		return;

	std::vector<d2d::IShape*>* shapes 
		= static_cast<std::vector<d2d::IShape*>*>(m_item->getUserData());
	for_each(shapes->begin(), shapes->end(), DeletePointerFunctor<d2d::IShape>());
	shapes->clear();
}

void StagePanel::changeCurrItem(LibraryItem* item)
{
	m_item = item;
	if (m_item && !m_item->getUserData())
	{
		std::vector<d2d::IShape*>* shapes = new std::vector<d2d::IShape*>;
		m_item->setUserData(shapes);
	}
	Refresh();
}
