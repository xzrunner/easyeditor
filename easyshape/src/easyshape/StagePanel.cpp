#include "StagePanel.h"
#include "StageCanvas.h"
#include "LibraryItem.h"
#include "ToolBarPanel.h"

namespace eshape
{

StagePanel::StagePanel(wxWindow* parent,
					   wxTopLevelWindow* frame)
	: EditPanel(parent, frame)
	, MultiShapesImpl(parent)
	, m_toolbar(NULL)
{
	m_canvas = new StageCanvas(this);
	m_item = NULL;
}

void StagePanel::clear()
{
	changeCurrItem(NULL);
	if (m_toolbar) {
		m_toolbar->changeCurrItem(NULL);
	}

	ee::EditPanel::clear();
	clearShapes();
}

void StagePanel::traverseShapes(ee::IVisitor& visitor, 
								ee::TraverseType type/* = ee::e_allExisting*/) const
{
	if (!m_item || !m_item->getUserData())
		return;

	std::vector<ee::IShape*>* shapes 
		= static_cast<std::vector<ee::IShape*>*>(m_item->getUserData());
	std::vector<ee::IShape*>::const_iterator itr = shapes->begin();
	for ( ; itr != shapes->end(); ++itr)
	{
		bool hasNext;
		visitor.visit(*itr, hasNext);
		if (!hasNext) break;
	}
}

void StagePanel::removeShape(ee::IShape* shape)
{
	if (!m_item || !m_item->getUserData())
		return;

	std::vector<ee::IShape*>* shapes 
		= static_cast<std::vector<ee::IShape*>*>(m_item->getUserData());
	std::vector<ee::IShape*>::iterator itr = shapes->begin();
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

void StagePanel::insertShape(ee::IShape* shape)
{
	if (!m_item || !m_item->getUserData())
		return;

	std::vector<ee::IShape*>* shapes 
		= static_cast<std::vector<ee::IShape*>*>(m_item->getUserData());
//	static_cast<libshape::ChainShape*>(shape)->setClosed(Context::Instance()->isClosed);
	shapes->push_back(shape);
}

void StagePanel::clearShapes()
{
	if (!m_item || !m_item->getUserData())
		return;

	std::vector<ee::IShape*>* shapes 
		= static_cast<std::vector<ee::IShape*>*>(m_item->getUserData());
	for_each(shapes->begin(), shapes->end(), DeletePointerFunctor<ee::IShape>());
	shapes->clear();
}

void StagePanel::changeCurrItem(LibraryItem* item)
{
	m_item = item;
	if (m_item && !m_item->getUserData())
	{
		std::vector<ee::IShape*>* shapes = new std::vector<ee::IShape*>;
		m_item->setUserData(shapes);
	}
	Refresh();
}

}