#ifndef _LIBSHAPE_EDIT_POLYLINES_OP_H_
#define _LIBSHAPE_EDIT_POLYLINES_OP_H_

#include <drag2d.h>

namespace libshape
{

class ChainShape;
class EditPolylinesCMPT;

class EditPolylinesOP : public d2d::SelectShapesOP
{
public:
	EditPolylinesOP(wxWindow* wnd, d2d::EditPanelImpl* stage, d2d::MultiShapesImpl* shapesImpl,
		EditPolylinesCMPT* cmpt);

	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

	virtual bool OnDraw() const;
	virtual bool Clear();

	void simplify();
	void updateFromSimplified();

private:
	void clearBuffer();

private:
	class UpdateChainVisitor : public d2d::Visitor
	{
	public:
		virtual void Visit(Object* object, bool& next);
	}; // UpdateChainVisitor

	class UpdateBufferVisitor : public d2d::Visitor
	{
	public:
		UpdateBufferVisitor(std::map<ChainShape*, ChainShape*>& simplifyBuffer);

		virtual void Visit(Object* object, bool& next);

	private:
		std::map<ChainShape*, ChainShape*>& m_simplifyBuffer;

	}; // UpdateBufferVisitor

	class OffsetVisitor : public d2d::Visitor
	{
	public:
		OffsetVisitor(const d2d::Vector& offset);

		virtual void Visit(d2d::Object* object, bool& next);

	private:
		const d2d::Vector& m_offset;

	}; // OffsetVisitor

private:
	EditPolylinesCMPT* m_cmpt;

	std::map<ChainShape*, ChainShape*> m_simplifyBuffer;

	d2d::Vector m_lastPos;

	bool m_bDirty;

}; // EditPolylinesOP

}

#endif // _LIBSHAPE_EDIT_POLYLINES_OP_H_