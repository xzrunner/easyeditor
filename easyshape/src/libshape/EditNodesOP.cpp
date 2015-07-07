#include "EditNodesOP.h"
#include "ChainShape.h"
#include "EditNodesAtomicOP.h"

namespace libshape
{

EditNodesOP::EditNodesOP(d2d::EditPanel* editPanel, d2d::MultiShapesImpl* shapesImpl)
	: SelectNodesOP(editPanel, shapesImpl, NULL, NULL)
{
	m_lastPos.setInvalid();
}

bool EditNodesOP::OnKeyDown(int keyCode)
{
	if (keyCode == WXK_DELETE)
	{
		for (size_t i = 0, n = m_nodeSelection.size(); i < n; ++i)
		{
			SelectNodesOP::ChainSelectedNodes* selected = m_nodeSelection[i];
			for (size_t j = 0, m = selected->selectedNodes.size(); j < m; ++j)
				selected->chain->Remove(selected->selectedNodes[j]);
		}
	}

	SelectNodesOP::OnKeyDown(keyCode);

	return false;
}

bool EditNodesOP::OnMouseLeftDown(int x, int y)
{
	if (SelectNodesOP::OnMouseLeftDown(x, y)) return true;

	if (wxGetKeyState(WXK_CONTROL)) return false;

	if (m_nodeSelection.empty())
		m_lastPos.setInvalid();
	else
		m_lastPos = m_stage->TransPosScrToProj(x, y);

	return false;
}

bool EditNodesOP::OnMouseDrag(int x, int y)
{
	if (SelectNodesOP::OnMouseDrag(x, y)) return true;

	if (wxGetKeyState(WXK_CONTROL)) return false;

	if (m_lastPos.isValid())
	{
		d2d::Vector currPos = m_stage->TransPosScrToProj(x, y);
		d2d::Vector offset = currPos - m_lastPos;
		for (size_t i = 0, n = m_nodeSelection.size(); i < n; ++i)
		{
			SelectNodesOP::ChainSelectedNodes* selected = m_nodeSelection[i];
			for (size_t j = 0, m = selected->selectedNodes.size(); j < m; ++j)
			{
				const d2d::Vector& from = selected->selectedNodes[j];
				d2d::Vector to = from + offset;
				selected->chain->Change(from, to);
				selected->selectedNodes[j] = to;
			}
			selected->chain->refresh();
		}
		m_lastPos = currPos;

		m_stage->Refresh();
	}

	return false;
}

bool EditNodesOP::OnDraw() const
{
	if (SelectNodesOP::OnDraw()) return true;

	const float radius = d2d::Settings::ctlPosSize == 0 ? 3 : d2d::Settings::ctlPosSize;
	for (size_t i = 0, n = m_buffer.size(); i < n; ++i)
	{
		d2d::PrimitiveDraw::drawPolyline(m_buffer[i].dst, d2d::Colorf(0.8f, 0.2f, 0.2f), false);
		d2d::PrimitiveDraw::drawCircles(m_buffer[i].dst, radius, true, 2, d2d::Colorf(0.2f, 0.2f, 0.8f));
	}

	return false;
}

bool EditNodesOP::Clear()
{
	if (SelectNodesOP::Clear()) return true;

	m_lastPos.setInvalid();
	m_buffer.clear();

	return false;
}

void EditNodesOP::simplify(float threshold)
{
	m_buffer.clear();
	for (size_t i = 0, n = m_nodeSelection.size(); i < n; ++i)
	{
		Modified modified;
		modified.src = m_nodeSelection[i];
		d2d::DouglasPeucker::implement(modified.src->selectedNodes, threshold, modified.dst);
		m_buffer.push_back(modified);
	}
}

void EditNodesOP::smooth(float samplingWidth)
{
	m_buffer.clear();
	for (size_t i = 0, n = m_nodeSelection.size(); i < n; ++i)
	{
		Modified modified;
		modified.src = m_nodeSelection[i];
		d2d::CosineSmooth::implement(modified.src->selectedNodes, samplingWidth, modified.dst);
		m_buffer.push_back(modified);
	}
}

void EditNodesOP::updateModified()
{
	std::vector<ChainShape*> chains;
	std::vector<std::vector<d2d::Vector> > chainsDst;

	for (size_t i = 0, n = m_buffer.size(); i < n; ++i)
	{
		const std::vector<d2d::Vector>& chain = m_buffer[i].src->chain->GetVertices();
		const std::vector<d2d::Vector>& src = m_buffer[i].src->selectedNodes;
		const std::vector<d2d::Vector>& dst = m_buffer[i].dst;

		size_t begin = 0, end = chain.size() - 1;
		for (size_t j = 0, m = chain.size(); j < m; ++j)
		{
			if (src.front() == chain[j])
			{
				begin = j;
				break;
			}
		}
		for (int j = chain.size() - 1; j >= 0; --j)
		{
			if (src.back() == chain[j])
			{
				end = j;
				break;
			}
		}

		assert(begin <= end);
		std::vector<d2d::Vector> result;
		copy(chain.begin(), chain.begin() + begin, back_inserter(result));
		copy(dst.begin(), dst.end(), back_inserter(result));
		copy(chain.begin() + end + 1, chain.end(), back_inserter(result));

		m_buffer[i].src->chain->Load(result);

		chains.push_back(m_buffer[i].src->chain);
		chainsDst.push_back(result);
	}

	m_stage->AddOpRecord(new edit_nodes::ModifyNodesAOP(chains, chainsDst));

	m_buffer.clear();
	clearSelectedNodes();
}

}