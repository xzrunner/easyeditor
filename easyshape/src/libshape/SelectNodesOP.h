#ifndef _LIBSHAPE_SELECT_NODES_OP_H_
#define _LIBSHAPE_SELECT_NODES_OP_H_

#include "DrawRectangleOP.h"

namespace libshape
{

class ChainShape;

class SelectNodesOP : public DrawRectangleOP
{
public:
	SelectNodesOP(d2d::EditPanel* editPanel, d2d::MultiShapesImpl* shapesImpl, 
		d2d::PropertySettingPanel* propertyPanel, d2d::AbstractEditCMPT* callback = NULL);
	virtual ~SelectNodesOP();

	virtual bool OnKeyDown(int keyCode);
	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);

	virtual bool OnDraw() const;
	virtual bool Clear();

	void fetchSelectedNode(std::vector<d2d::Vector>& nodes) const;

	static int getThreshold();

protected:
	void clearSelectedNodes();

private:
	void OnDirectionKeyDown(d2d::DirectionType type);

protected:
	struct ChainSelectedNodes
	{
	public:
		ChainShape* chain;
		std::vector<d2d::Vector> selectedNodes;
	}; // ChainSelectedNodes

private:
	class PosQueryVisitor : public d2d::IVisitor
	{
	public:
		PosQueryVisitor(const d2d::Vector& pos, ChainSelectedNodes** result);
		virtual void visit(Object* object, bool& bFetchNext);

	private:
		const d2d::Vector& m_pos;
		d2d::Rect m_rect;
		ChainSelectedNodes** m_result;

	}; // PosQueryVisitor

	class RectQueryVisitor : public d2d::IVisitor
	{
	public:
		RectQueryVisitor(const d2d::Rect& rect, std::vector<ChainSelectedNodes*>& result);
		virtual void visit(Object* object, bool& bFetchNext);

	private:
		const d2d::Rect& m_rect;
		std::vector<ChainSelectedNodes*>& m_result;

	}; // RectQueryVisitor

protected:
	std::vector<ChainSelectedNodes*> m_nodeSelection;

private:
	d2d::MultiShapesImpl* m_shapeImpl;

	d2d::Vector m_firstPos;

}; // SelectNodesOP

}

#endif // _LIBSHAPE_SELECT_NODES_OP_H_