#ifndef _EASYSHAPE_SELECT_NODES_OP_H_
#define _EASYSHAPE_SELECT_NODES_OP_H_

#include "DrawRectangleOP.h"

namespace eshape
{

class ChainShape;

class SelectNodesOP : public DrawRectangleOP
{
public:
	SelectNodesOP(wxWindow* wnd, ee::EditPanelImpl* stage, ee::MultiShapesImpl* shapesImpl, 
		ee::EditCMPT* callback = NULL);
	virtual ~SelectNodesOP();

	virtual bool OnKeyDown(int keyCode);
	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);

	virtual bool OnDraw() const;
	virtual bool Clear();

	void fetchSelectedNode(std::vector<ee::Vector>& nodes) const;

	static int getThreshold();

protected:
	void clearSelectedNodes();

private:
	void OnDirectionKeyDown(ee::DirectionType type);

protected:
	struct ChainSelectedNodes
	{
	public:
		ChainShape* chain;
		std::vector<ee::Vector> selectedNodes;
	}; // ChainSelectedNodes

private:
	class PosQueryVisitor : public ee::Visitor
	{
	public:
		PosQueryVisitor(const ee::Vector& pos, ChainSelectedNodes** result);
		virtual void Visit(Object* object, bool& next);

	private:
		const ee::Vector& m_pos;
		ee::Rect m_rect;
		ChainSelectedNodes** m_result;

	}; // PosQueryVisitor

	class RectQueryVisitor : public ee::Visitor
	{
	public:
		RectQueryVisitor(const ee::Rect& rect, std::vector<ChainSelectedNodes*>& result);
		virtual void Visit(Object* object, bool& next);

	private:
		const ee::Rect& m_rect;
		std::vector<ChainSelectedNodes*>& m_result;

	}; // RectQueryVisitor

protected:
	std::vector<ChainSelectedNodes*> m_nodeSelection;

private:
	ee::MultiShapesImpl* m_shapeImpl;

	ee::Vector m_firstPos;

}; // SelectNodesOP

}

#endif // _EASYSHAPE_SELECT_NODES_OP_H_