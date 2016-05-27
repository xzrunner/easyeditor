#ifndef _EASYSHAPE_SELECT_NODES_OP_H_
#define _EASYSHAPE_SELECT_NODES_OP_H_

#include "DrawRectangleOP.h"

#include <ee/common_type.h>
#include <ee/Visitor.h>

#include <SM_Rect.h>

#include <vector>

namespace ee { class MultiShapesImpl; class EditCMPT; }

namespace eshape
{

class PolylineShape;

class SelectNodesOP : public DrawRectangleOP
{
public:
	SelectNodesOP(wxWindow* wnd, ee::EditPanelImpl* stage, ee::MultiShapesImpl* shapes_impl, 
		ee::EditCMPT* callback = NULL);
	virtual ~SelectNodesOP();

	virtual bool OnKeyDown(int keyCode);
	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);

	virtual bool OnDraw() const;
	virtual bool Clear();

	void FetchSelectedNode(std::vector<sm::vec2>& nodes) const;

	static int GetThreshold();

protected:
	void ClearSelectedNodes();

private:
	void OnDirectionKeyDown(ee::DirectionType type);

protected:
	struct ChainSelectedNodes
	{
	public:
		PolylineShape* polyline;
		std::vector<sm::vec2> selectedNodes;
	}; // ChainSelectedNodes

private:
	class PosQueryVisitor : public ee::Visitor
	{
	public:
		PosQueryVisitor(const sm::vec2& pos, ChainSelectedNodes** result);
		virtual void Visit(Object* object, bool& next);

	private:
		const sm::vec2& m_pos;
		sm::rect m_rect;
		ChainSelectedNodes** m_result;

	}; // PosQueryVisitor

	class RectQueryVisitor : public ee::Visitor
	{
	public:
		RectQueryVisitor(const sm::rect& rect, std::vector<ChainSelectedNodes*>& result);
		virtual void Visit(Object* object, bool& next);

	private:
		const sm::rect& m_rect;
		std::vector<ChainSelectedNodes*>& m_result;

	}; // RectQueryVisitor

protected:
	std::vector<ChainSelectedNodes*> m_node_selection;

private:
	ee::MultiShapesImpl* m_shape_impl;

	sm::vec2 m_first_pos;
	
}; // SelectNodesOP

}

#endif // _EASYSHAPE_SELECT_NODES_OP_H_