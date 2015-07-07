#include "SelectNodesOP.h"
#include "ChainShape.h"
#include "Math.h"

namespace libshape
{

SelectNodesOP::SelectNodesOP(d2d::EditPanel* editPanel, d2d::MultiShapesImpl* shapesImpl,
							 d2d::ViewPanelMgr* view_panel_mgr,
							 d2d::AbstractEditCMPT* callback /*= NULL*/)
	: DrawRectangleOP(editPanel)
	, m_shapeImpl(shapesImpl)
{
	m_firstPos.setInvalid();
}

SelectNodesOP::~SelectNodesOP()
{
	clearSelectedNodes();
}

bool SelectNodesOP::OnKeyDown(int keyCode)
{
	if (DrawRectangleOP::OnKeyDown(keyCode)) return true;

	switch (keyCode)
	{
	case WXK_DELETE:
		clearSelectedNodes();
		m_stage->Refresh();
		break;
	case 'a': case 'A':
		OnDirectionKeyDown(d2d::e_left);
		m_stage->Refresh();
		break;
	case 'd': case 'D':
		OnDirectionKeyDown(d2d::e_right);
		m_stage->Refresh();
		break;
	case 's': case 'S':
		OnDirectionKeyDown(d2d::e_down);
		m_stage->Refresh();
		break;
	case 'w': case 'W':
		OnDirectionKeyDown(d2d::e_up);
		m_stage->Refresh();
		break;
	}

	return false;
}

bool SelectNodesOP::OnMouseLeftDown(int x, int y)
{
	d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
	ChainSelectedNodes* selected = NULL;
	m_shapeImpl->traverseShapes(PosQueryVisitor(pos, &selected), d2d::DT_VISIBLE);
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
					if (d2d::Math::getDistance(pos, chainNodes->selectedNodes[j]) < getThreshold())
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
					if (d2d::Math::getDistance(pos, chainNodes->selectedNodes[j]) < getThreshold())
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
		DrawRectangleOP::OnMouseLeftDown(x, y);
		m_firstPos = pos;
		if (!wxGetKeyState(WXK_CONTROL))
			clearSelectedNodes();
		m_stage->Refresh();
	}

	return false;
}

bool SelectNodesOP::OnMouseLeftUp(int x, int y)
{
	if (DrawRectangleOP::OnMouseLeftUp(x, y)) return true;

	if (m_firstPos.isValid())
	{
		d2d::Rect rect(m_firstPos, m_stage->TransPosScrToProj(x, y));
		m_shapeImpl->traverseShapes(RectQueryVisitor(rect, m_nodeSelection), 
			d2d::DT_SELECTABLE);

		m_firstPos.setInvalid();
	}

	return false;
}

bool SelectNodesOP::OnDraw() const
{
	if (DrawRectangleOP::OnDraw()) return true;

	if (m_nodeSelection.empty()) return false;

	std::vector<d2d::Vector> nodes;
	int count = 0;
	for (size_t i = 0, n = m_nodeSelection.size(); i < n; ++i)
		count += m_nodeSelection[i]->selectedNodes.size();
	nodes.reserve(count);

	for (size_t i = 0, n = m_nodeSelection.size(); i < n; ++i)
	{
		const std::vector<d2d::Vector>& selectedNodes = m_nodeSelection[i]->selectedNodes;
		copy(selectedNodes.begin(), selectedNodes.end(), back_inserter(nodes));
	}

	d2d::PrimitiveDraw::drawCircles(nodes, getThreshold(), true, 2, d2d::Colorf(0.8f, 0.4f, 0.4f));

	return false;
}

bool SelectNodesOP::Clear()
{
	if (DrawRectangleOP::Clear()) return true;

	clearSelectedNodes();
	m_firstPos.setInvalid();

	return false;
}

void SelectNodesOP::fetchSelectedNode(std::vector<d2d::Vector>& nodes) const
{
	if (m_nodeSelection.empty()) return;

	std::vector<ChainShape*> src;
	src.reserve(m_nodeSelection.size());
	for (size_t i = 0, n = m_nodeSelection.size(); i < n; ++i)
		src.push_back(new ChainShape(m_nodeSelection[i]->selectedNodes, false));

	Math::mergeMultiChains(src, nodes);

	for (size_t i = 0, n = src.size(); i < n; ++i)
		src[i]->Release();
}

int SelectNodesOP::getThreshold()
{
	return d2d::Settings::ctlPosSize == 0 ? 3 : d2d::Settings::ctlPosSize;
}

void SelectNodesOP::clearSelectedNodes()
{
	for_each(m_nodeSelection.begin(), m_nodeSelection.end(), DeletePointerFunctor<ChainSelectedNodes>());
	m_nodeSelection.clear();
}

void SelectNodesOP::OnDirectionKeyDown(d2d::DirectionType type)
{
	if (m_nodeSelection.empty()) {
		return;
	}

	d2d::Vector offset;
	switch (type)
	{
	case d2d::e_left:
		offset.x -= 1;
		break;
	case d2d::e_right:
		offset.x += 1;
		break;
	case d2d::e_down:
		offset.y -= 1;
		break;
	case d2d::e_up:
		offset.y += 1;
		break;
	}

	for (int i = 0, n = m_nodeSelection.size(); i < n; ++i) {
		ChainSelectedNodes* nodes = m_nodeSelection[i];
		for (int j = 0, m = nodes->selectedNodes.size(); j < m; ++j) {
			const d2d::Vector& from = nodes->selectedNodes[j];
			d2d::Vector to = from + offset;
			nodes->chain->Change(from, to);
			nodes->selectedNodes[j] = to;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// class SelectNodesOP::PosQueryVisitor
//////////////////////////////////////////////////////////////////////////

SelectNodesOP::PosQueryVisitor::
PosQueryVisitor(const d2d::Vector& pos, ChainSelectedNodes** result)
	: m_pos(pos)
	, m_rect(pos, SelectNodesOP::getThreshold(), SelectNodesOP::getThreshold())
	, m_result(result)
{
}

void SelectNodesOP::PosQueryVisitor::
Visit(Object* object, bool& bFetchNext)
{
	ChainShape* chain = static_cast<ChainShape*>(object);

	if (d2d::Math::isRectIntersectRect(chain->getRect(), m_rect))
	{
		const std::vector<d2d::Vector>& vertices = chain->GetVertices();
		for (size_t i = 0, n = vertices.size(); i < n; ++i)
		{
			if (d2d::Math::getDistance(m_pos, vertices[i]) < SelectNodesOP::getThreshold())
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
RectQueryVisitor(const d2d::Rect& rect, std::vector<ChainSelectedNodes*>& result)
	: m_rect(rect)
	, m_result(result)
{
}

void SelectNodesOP::RectQueryVisitor::
Visit(Object* object, bool& bFetchNext)
{
	ChainShape* chain = static_cast<ChainShape*>(object);

	if (d2d::Math::isRectIntersectRect(chain->getRect(), m_rect))
	{
		ChainSelectedNodes* result = new ChainSelectedNodes;
		result->chain = chain;

		const std::vector<d2d::Vector>& vertices = chain->GetVertices();
		for (size_t i = 0, n = vertices.size(); i < n; ++i)
		{
			if (d2d::Math::isPointInRect(vertices[i], m_rect))
				result->selectedNodes.push_back(vertices[i]);
		}

		if (result->selectedNodes.empty())
			delete result;
		else
			m_result.push_back(result);
	}

	bFetchNext = true;
}

}