#ifndef _EASYSHAPE_SELECT_NODES_OP_H_
#define _EASYSHAPE_SELECT_NODES_OP_H_

#include "DrawRectangleOP.h"

#include <ee/key_type.h>
#include <ee/Visitor.h>
#include <ee/Shape.h>

#include <cu/cu_stl.h>
#include <SM_Rect.h>

#include <vector>

namespace ee { class MultiShapesImpl; class EditCMPT; }

namespace eshape
{

class EditedPolyShape;

class SelectNodesOP : public DrawRectangleOP
{
public:
	SelectNodesOP(wxWindow* wnd, ee::EditPanelImpl* stage, ee::MultiShapesImpl* shapes_impl, 
		ee::EditCMPT* callback = NULL);
	virtual ~SelectNodesOP();

	virtual bool OnKeyDown(int keyCode) override;
	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseLeftUp(int x, int y) override;

	virtual bool OnDraw() const override;
	virtual bool Clear() override;

	void FetchSelectedNode(CU_VEC<sm::vec2>& nodes) const;

	static int GetThreshold();

protected:
	void ClearSelectedNodes();

private:
	void OnDirectionKeyDown(int type);

protected:
	struct ChainSelectedNodes
	{
	public:
		std::shared_ptr<EditedPolyShape> polyline;
		CU_VEC<sm::vec2> selectedNodes;
	}; // ChainSelectedNodes

private:
	class PosQueryVisitor : public ee::RefVisitor<ee::Shape>
	{
	public:
		PosQueryVisitor(const sm::vec2& pos, ChainSelectedNodes** result);
		virtual void Visit(const ee::ShapePtr& shape, bool& next) override;

	private:
		const sm::vec2& m_pos;
		sm::rect m_rect;
		ChainSelectedNodes** m_result;

	}; // PosQueryVisitor

	class RectQueryVisitor : public ee::RefVisitor<ee::Shape>
	{
	public:
		RectQueryVisitor(const sm::rect& rect, CU_VEC<ChainSelectedNodes*>& result);
		virtual void Visit(const ee::ShapePtr& shape, bool& next) override;

	private:
		const sm::rect& m_rect;
		CU_VEC<ChainSelectedNodes*>& m_result;

	}; // RectQueryVisitor

protected:
	CU_VEC<ChainSelectedNodes*> m_node_selection;

private:
	ee::MultiShapesImpl* m_shape_impl;

	sm::vec2 m_first_pos;
	
}; // SelectNodesOP

}

#endif // _EASYSHAPE_SELECT_NODES_OP_H_