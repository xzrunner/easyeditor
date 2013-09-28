#include "SelectNodesOP.h"

#include "dataset/ChainShape.h"
#include "view/MultiShapesImpl.h"
#include "common/Settings.h"
#include "common/Math.h"
#include "common/tools.h"
#include "render/PrimitiveDraw.h"

namespace d2d
{

SelectNodesOP::SelectNodesOP(EditPanel* editPanel, MultiShapesImpl* shapesImpl,
							 PropertySettingPanel* propertyPanel, 
							 AbstractEditCMPT* callback /*= NULL*/)
	: DrawRectangleOP(editPanel)
	, m_shapeImpl(shapesImpl)
{
	m_firstPos.setInvalid();
}

SelectNodesOP::~SelectNodesOP()
{
	clearSelectedNodes();
}

bool SelectNodesOP::onKeyDown(int keyCode)
{
	if (DrawRectangleOP::onKeyDown(keyCode)) return true;

	if (keyCode == WXK_DELETE)
	{
		clearSelectedNodes();
		m_editPanel->Refresh();
	}

	return false;
}

bool SelectNodesOP::onMouseLeftDown(int x, int y)
{
	Vector pos = m_editPanel->transPosScreenToProject(x, y);
	ChainSelectedNodes* selected = NULL;
	m_shapeImpl->traverseShapes(PosQueryVisitor(pos, &selected), e_visible);
	if (selected)
	{
		if (wxGetKeyState(WXK_CONTROL))
		{
			bool isExist = false;
			for (size_t i = 0, n = m_nodeSelection.size(); i < n && !isExist; ++i)
			{
				ChainSelectedNodes* chainNodes = m_nodeSelection[i];
				if (chainNodes->chain != selected->chain) continue;
				for (size_t j = 0, m = chainNodes->selectedNodes.size(); j < m && !isExist; ++j)
				{
					if (Math::getDistance(pos, chainNodes->selectedNodes[j]) < getThreshold())
					{
						chainNodes->selectedNodes.erase(chainNodes->selectedNodes.begin() + j);
						if (chainNodes->selectedNodes.empty())
							m_nodeSelection.erase(m_nodeSelection.begin() + i);
						isExist = true;
					}
				}
			}

			if (!isExist)
				m_nodeSelection.push_back(selected);
			else
				delete selected;
		}
		else
		{
			bool isExist = false;
			for (size_t i = 0, n = m_nodeSelection.size(); i < n && !isExist; ++i)
			{
				ChainSelectedNodes* chainNodes = m_nodeSelection[i];
				if (chainNodes->chain != selected->chain) continue;
				for (size_t j = 0, m = chainNodes->selectedNodes.size(); j < m && !isExist; ++j)
				{
					if (Math::getDistance(pos, chainNodes->selectedNodes[j]) < getThreshold())
						isExist = true;
				}
			}

			if (!isExist)
			{
				clearSelectedNodes();
				m_nodeSelection.push_back(selected);
			}
			else
				delete selected;
		}

		m_firstPos.setInvalid();
	}
	else
	{
		DrawRectangleOP::onMouseLeftDown(x, y);
		m_firstPos = pos;
		if (!wxGetKeyState(WXK_CONTROL))
			clearSelectedNodes();
		m_editPanel->Refresh();
	}

	return false;
}

bool SelectNodesOP::onMouseLeftUp(int x, int y)
{
	if (DrawRectangleOP::onMouseLeftUp(x, y)) return true;

	if (m_firstPos.isValid())
	{
		Rect rect(m_firstPos, m_editPanel->transPosScreenToProject(x, y));
		m_shapeImpl->traverseShapes(RectQueryVisitor(rect, m_nodeSelection), 
			e_selectable);

		m_firstPos.setInvalid();
	}

	return false;
}

bool SelectNodesOP::onDraw() const
{
	if (DrawRectangleOP::onDraw()) return true;

	if (m_nodeSelection.empty()) return false;

	std::vector<Vector> nodes;
	int count = 0;
	for (size_t i = 0, n = m_nodeSelection.size(); i < n; ++i)
		count += m_nodeSelection[i]->selectedNodes.size();
	nodes.reserve(count);

	for (size_t i = 0, n = m_nodeSelection.size(); i < n; ++i)
	{
		const std::vector<Vector>& selectedNodes = m_nodeSelection[i]->selectedNodes;
		copy(selectedNodes.begin(), selectedNodes.end(), back_inserter(nodes));
	}

	PrimitiveDraw::drawCircles(nodes, getThreshold(), true, 2, Colorf(0.8f, 0.4f, 0.4f));

	return false;
}

bool SelectNodesOP::clear()
{
	if (DrawRectangleOP::clear()) return true;

	clearSelectedNodes();
	m_firstPos.setInvalid();

	return false;
}

void SelectNodesOP::fetchSelectedNode(std::vector<Vector>& nodes) const
{
	if (m_nodeSelection.empty()) return;

	std::vector<ChainShape*> src;
	src.reserve(m_nodeSelection.size());
	for (size_t i = 0, n = m_nodeSelection.size(); i < n; ++i)
		src.push_back(new ChainShape(m_nodeSelection[i]->selectedNodes, false));

	Math::mergeMultiChains(src, nodes);

	for (size_t i = 0, n = src.size(); i < n; ++i)
		src[i]->release();
}

int SelectNodesOP::getThreshold()
{
	return Settings::ctlPosSize == 0 ? 3 : Settings::ctlPosSize;
}

void SelectNodesOP::clearSelectedNodes()
{
	for_each(m_nodeSelection.begin(), m_nodeSelection.end(), DeletePointerFunctor<ChainSelectedNodes>());
	m_nodeSelection.clear();
}

//////////////////////////////////////////////////////////////////////////
// class SelectNodesOP::PosQueryVisitor
//////////////////////////////////////////////////////////////////////////

SelectNodesOP::PosQueryVisitor::
PosQueryVisitor(const Vector& pos, ChainSelectedNodes** result)
	: m_pos(pos)
	, m_rect(pos, SelectNodesOP::getThreshold(), SelectNodesOP::getThreshold())
	, m_result(result)
{
}

void SelectNodesOP::PosQueryVisitor::
visit(Object* object, bool& bFetchNext)
{
	ChainShape* chain = static_cast<ChainShape*>(object);

	if (Math::isRectIntersectRect(chain->getRect(), m_rect))
	{
		const std::vector<Vector>& vertices = chain->getVertices();
		for (size_t i = 0, n = vertices.size(); i < n; ++i)
		{
			if (Math::getDistance(m_pos, vertices[i]) < SelectNodesOP::getThreshold())
			{
				ChainSelectedNodes* result = new ChainSelectedNodes;
				result->chain = chain;
				result->selectedNodes.push_back(vertices[i]);
				*m_result = result;

				bFetchNext = false;
				return;
			}
		}
	}

	bFetchNext = true;
}

//////////////////////////////////////////////////////////////////////////
// class SelectNodesOP::RectQueryVisitor
//////////////////////////////////////////////////////////////////////////

SelectNodesOP::RectQueryVisitor::
RectQueryVisitor(const Rect& rect, std::vector<ChainSelectedNodes*>& result)
	: m_rect(rect)
	, m_result(result)
{
}

void SelectNodesOP::RectQueryVisitor::
visit(Object* object, bool& bFetchNext)
{
	ChainShape* chain = static_cast<ChainShape*>(object);

	if (Math::isRectIntersectRect(chain->getRect(), m_rect))
	{
		ChainSelectedNodes* result = new ChainSelectedNodes;
		result->chain = chain;

		const std::vector<Vector>& vertices = chain->getVertices();
		for (size_t i = 0, n = vertices.size(); i < n; ++i)
		{
			if (Math::isPointInRect(vertices[i], m_rect))
				result->selectedNodes.push_back(vertices[i]);
		}

		if (result->selectedNodes.empty())
			delete result;
		else
			m_result.push_back(result);
	}

	bFetchNext = true;
}

} // d2d