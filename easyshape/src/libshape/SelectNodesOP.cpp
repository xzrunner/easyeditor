#include "SelectNodesOP.h"
#include "ChainShape.h"
#include "Math.h"

#include <ee/EditPanelImpl.h>
#include <ee/MultiShapesImpl.h>
#include <ee/Math2D.h>
#include <ee/EE_RVG.h>
#include <ee/SettingData.h>
#include <ee/std_functor.h>
#include <ee/panel_msg.h>

#include <algorithm>

namespace eshape
{

SelectNodesOP::SelectNodesOP(wxWindow* wnd, ee::EditPanelImpl* stage, 
							 ee::MultiShapesImpl* shapes_impl,
							 ee::EditCMPT* callback /*= NULL*/)
	: DrawRectangleOP(wnd, stage)
	, m_shape_impl(shapes_impl)
{
	m_first_pos.SetInvalid();
}

SelectNodesOP::~SelectNodesOP()
{
	ClearSelectedNodes();
}

bool SelectNodesOP::OnKeyDown(int keyCode)
{
	if (DrawRectangleOP::OnKeyDown(keyCode)) return true;

	switch (keyCode)
	{
	case WXK_DELETE:
		ClearSelectedNodes();
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
	m_shape_impl->TraverseShapes(PosQueryVisitor(pos, &selected), ee::DT_VISIBLE);
	if (selected)
	{
		if (m_stage->GetKeyState(WXK_CONTROL))
		{
			bool isExist = false;
			for (size_t i = 0, n = m_node_selection.size(); i < n && !isExist; ++i)
			{
				ChainSelectedNodes* chainNodes = m_node_selection[i];
				if (chainNodes->chain != selected->chain) continue;
				for (size_t j = 0, m = chainNodes->selectedNodes.size(); j < m && !isExist; ++j)
				{
					if (ee::Math2D::GetDistance(pos, chainNodes->selectedNodes[j]) < GetThreshold())
					{
						chainNodes->selectedNodes.erase(chainNodes->selectedNodes.begin() + j);
						if (chainNodes->selectedNodes.empty())
							m_node_selection.erase(m_node_selection.begin() + i);
						isExist = true;
					}
				}
			}

			if (!isExist)
				m_node_selection.push_back(selected);
			else
				delete selected;
		}
		else
		{
			bool isExist = false;
			for (size_t i = 0, n = m_node_selection.size(); i < n && !isExist; ++i)
			{
				ChainSelectedNodes* chainNodes = m_node_selection[i];
				if (chainNodes->chain != selected->chain) continue;
				for (size_t j = 0, m = chainNodes->selectedNodes.size(); j < m && !isExist; ++j)
				{
					if (ee::Math2D::GetDistance(pos, chainNodes->selectedNodes[j]) < GetThreshold())
						isExist = true;
				}
			}

			if (!isExist)
			{
				ClearSelectedNodes();
				m_node_selection.push_back(selected);
			}
			else
				delete selected;
		}

		m_first_pos.SetInvalid();
	}
	else
	{
		DrawRectangleOP::OnMouseLeftDown(x, y);
		m_first_pos = pos;
		if (!m_stage->GetKeyState(WXK_CONTROL))
			ClearSelectedNodes();

	}

	return false;
}

bool SelectNodesOP::OnMouseLeftUp(int x, int y)
{
	if (DrawRectangleOP::OnMouseLeftUp(x, y)) return true;

	if (m_first_pos.IsValid())
	{
		ee::Rect rect(m_first_pos, m_stage->TransPosScrToProj(x, y));
		m_shape_impl->TraverseShapes(RectQueryVisitor(rect, m_node_selection), 
			ee::DT_SELECTABLE);

		m_first_pos.SetInvalid();
	}

	return false;
}

bool SelectNodesOP::OnDraw() const
{
	if (DrawRectangleOP::OnDraw()) return true;

	if (m_node_selection.empty()) return false;

	std::vector<ee::Vector> nodes;
	int count = 0;
	for (size_t i = 0, n = m_node_selection.size(); i < n; ++i)
		count += m_node_selection[i]->selectedNodes.size();
	nodes.reserve(count);

	for (size_t i = 0, n = m_node_selection.size(); i < n; ++i)
	{
		const std::vector<ee::Vector>& selectedNodes = m_node_selection[i]->selectedNodes;
		copy(selectedNodes.begin(), selectedNodes.end(), back_inserter(nodes));
	}

	ee::RVG::Circles(nodes, GetThreshold(), true, 2, ee::Colorf(0.8f, 0.4f, 0.4f));

	return false;
}

bool SelectNodesOP::Clear()
{
	if (DrawRectangleOP::Clear()) return true;

	ClearSelectedNodes();
	m_first_pos.SetInvalid();

	return false;
}

void SelectNodesOP::FetchSelectedNode(std::vector<ee::Vector>& nodes) const
{
	if (m_node_selection.empty()) return;

	std::vector<ChainShape*> src;
	src.reserve(m_node_selection.size());
	for (size_t i = 0, n = m_node_selection.size(); i < n; ++i)
		src.push_back(new ChainShape(m_node_selection[i]->selectedNodes, false));

	Math::mergeMultiChains(src, nodes);

	for (size_t i = 0, n = src.size(); i < n; ++i)
		src[i]->Release();
}

int SelectNodesOP::GetThreshold()
{
	return ee::SettingData::ctl_pos_sz == 0 ? 3 : ee::SettingData::ctl_pos_sz;
}

void SelectNodesOP::ClearSelectedNodes()
{
	for_each(m_node_selection.begin(), m_node_selection.end(), ee::DeletePointerFunctor<ChainSelectedNodes>());
	m_node_selection.clear();
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void SelectNodesOP::OnDirectionKeyDown(ee::DirectionType type)
{
	if (m_node_selection.empty()) {
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
	for (int i = 0, n = m_node_selection.size(); i < n; ++i) {
		ChainSelectedNodes* nodes = m_node_selection[i];
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
	, m_rect(pos, SelectNodesOP::GetThreshold(), SelectNodesOP::GetThreshold())
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
			if (ee::Math2D::GetDistance(m_pos, vertices[i]) < SelectNodesOP::GetThreshold())
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