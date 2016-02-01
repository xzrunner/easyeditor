#include "SelectNodesOP.h"
#include "ChainShape.h"
#include "Math.h"

namespace eshape
{

SelectNodesOP::SelectNodesOP(wxWindow* wnd, ee::EditPanelImpl* stage, 
							 ee::MultiShapesImpl* shapesImpl,
							 ee::EditCMPT* callback /*= NULL*/)
	: DrawRectangleOP(wnd, stage)
	, m_shapeImpl(shapesImpl)
{
	m_firstPos.SetInvalid();
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
		break;
	case 'a': case 'A':
		OnDirectionKeyDown(ee::e_left);
		break;
	case 'd': case 'D':
		OnDirectionKeyDown(ee::e_right);
		break;
	case 's': case 'S':
		OnDirectionKeyDown(ee::e_down);
		break;
	case 'w': case 'W':
		OnDirectionKeyDown(ee::e_up);
		break;
	}

	return false;
}

bool SelectNodesOP::OnMouseLeftDown(int x, int y)
{
	ee::Vector pos = m_stage->TransPosScrToProj(x, y);
	ChainSelectedNodes* selected = NULL;
	m_shapeImpl->TraverseShapes(PosQueryVisitor(pos, &selected), ee::DT_VISIBLE);
	if (selected)
	{
		if (m_stage->GetKeyState(WXK_CONTROL))
		{
			bool isExist = false;
			for (size_t i = 0, n = m_nodeSelection.size(); i < n && !isExist; ++i)
			{
				ChainSelectedNodes* chainNodes = m_nodeSelection[i];
				if (chainNodes->chain != selected->chain) continue;
				for (size_t j = 0, m = chainNodes->selectedNodes.size(); j < m && !isExist; ++j)
				{
					if (ee::Math2D::GetDistance(pos, chainNodes->selectedNodes[j]) < getThreshold())
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
					if (ee::Math2D::GetDistance(pos, chainNodes->selectedNodes[j]) < getThreshold())
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

		m_firstPos.SetInvalid();
	}
	else
	{
		DrawRectangleOP::OnMouseLeftDown(x, y);
		m_firstPos = pos;
		if (!m_stage->GetKeyState(WXK_CONTROL))
			clearSelectedNodes();

	}

	return false;
}

bool SelectNodesOP::OnMouseLeftUp(int x, int y)
{
	if (DrawRectangleOP::OnMouseLeftUp(x, y)) return true;

	if (m_firstPos.IsValid())
	{
		ee::Rect rect(m_firstPos, m_stage->TransPosScrToProj(x, y));
		m_shapeImpl->TraverseShapes(RectQueryVisitor(rect, m_nodeSelection), 
			ee::DT_SELECTABLE);

		m_firstPos.SetInvalid();
	}

	return false;
}

bool SelectNodesOP::OnDraw() const
{
	if (DrawRectangleOP::OnDraw()) return true;

	if (m_nodeSelection.empty()) return false;

	std::vector<ee::Vector> nodes;
	int count = 0;
	for (size_t i = 0, n = m_nodeSelection.size(); i < n; ++i)
		count += m_nodeSelection[i]->selectedNodes.size();
	nodes.reserve(count);

	for (size_t i = 0, n = m_nodeSelection.size(); i < n; ++i)
	{
		const std::vector<ee::Vector>& selectedNodes = m_nodeSelection[i]->selectedNodes;
		copy(selectedNodes.begin(), selectedNodes.end(), back_inserter(nodes));
	}

	ee::PrimitiveDraw::DrawCircles(nodes, getThreshold(), true, 2, ee::Colorf(0.8f, 0.4f, 0.4f));

	return false;
}

bool SelectNodesOP::Clear()
{
	if (DrawRectangleOP::Clear()) return true;

	clearSelectedNodes();
	m_firstPos.SetInvalid();

	return false;
}

void SelectNodesOP::fetchSelectedNode(std::vector<ee::Vector>& nodes) const
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
	return ee::SettingData::ctl_pos_sz == 0 ? 3 : ee::SettingData::ctl_pos_sz;
}

void SelectNodesOP::clearSelectedNodes()
{
	for_each(m_nodeSelection.begin(), m_nodeSelection.end(), DeletePointerFunctor<ChainSelectedNodes>());
	m_nodeSelection.clear();
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void SelectNodesOP::OnDirectionKeyDown(ee::DirectionType type)
{
	if (m_nodeSelection.empty()) {
		return;
	}

	ee::Vector offset;
	switch (type)
	{
	case ee::e_left:
		offset.x -= 1;
		break;
	case ee::e_right:
		offset.x += 1;
		break;
	case ee::e_down:
		offset.y -= 1;
		break;
	case ee::e_up:
		offset.y += 1;
		break;
	}

	bool dirty = false;
	for (int i = 0, n = m_nodeSelection.size(); i < n; ++i) {
		ChainSelectedNodes* nodes = m_nodeSelection[i];
		for (int j = 0, m = nodes->selectedNodes.size(); j < m; ++j) {
			const ee::Vector& from = nodes->selectedNodes[j];
			ee::Vector to = from + offset;
			nodes->chain->Change(from, to);
			nodes->selectedNodes[j] = to;
			dirty = true;
		}
	}

	if (dirty) {
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

//////////////////////////////////////////////////////////////////////////
// class SelectNodesOP::PosQueryVisitor
//////////////////////////////////////////////////////////////////////////

SelectNodesOP::PosQueryVisitor::
PosQueryVisitor(const ee::Vector& pos, ChainSelectedNodes** result)
	: m_pos(pos)
	, m_rect(pos, SelectNodesOP::getThreshold(), SelectNodesOP::getThreshold())
	, m_result(result)
{
}

void SelectNodesOP::PosQueryVisitor::
Visit(Object* object, bool& next)
{
	ChainShape* chain = static_cast<ChainShape*>(object);

	if (ee::Math2D::IsRectIntersectRect(chain->GetRect(), m_rect))
	{
		const std::vector<ee::Vector>& vertices = chain->GetVertices();
		for (size_t i = 0, n = vertices.size(); i < n; ++i)
		{
			if (ee::Math2D::GetDistance(m_pos, vertices[i]) < SelectNodesOP::getThreshold())
			{
				ChainSelectedNodes* result = new ChainSelectedNodes;
				result->chain = chain;
				result->selectedNodes.push_back(vertices[i]);
				*m_result = result;

				next = false;
				return;
			}
		}
	}

	next = true;
}

//////////////////////////////////////////////////////////////////////////
// class SelectNodesOP::RectQueryVisitor
//////////////////////////////////////////////////////////////////////////

SelectNodesOP::RectQueryVisitor::
RectQueryVisitor(const ee::Rect& rect, std::vector<ChainSelectedNodes*>& result)
	: m_rect(rect)
	, m_result(result)
{
}

void SelectNodesOP::RectQueryVisitor::
Visit(Object* object, bool& next)
{
	ChainShape* chain = static_cast<ChainShape*>(object);

	if (ee::Math2D::IsRectIntersectRect(chain->GetRect(), m_rect))
	{
		ChainSelectedNodes* result = new ChainSelectedNodes;
		result->chain = chain;

		const std::vector<ee::Vector>& vertices = chain->GetVertices();
		for (size_t i = 0, n = vertices.size(); i < n; ++i)
		{
			if (ee::Math2D::IsPointInRect(vertices[i], m_rect))
				result->selectedNodes.push_back(vertices[i]);
		}

		if (result->selectedNodes.empty())
			delete result;
		else
			m_result.push_back(result);
	}

	next = true;
}

}