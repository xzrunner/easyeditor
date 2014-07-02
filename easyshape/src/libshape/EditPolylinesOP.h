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
	EditPolylinesOP(d2d::EditPanel* editPanel, d2d::MultiShapesImpl* shapesImpl,
		d2d::PropertySettingPanel* propertyPanel, EditPolylinesCMPT* cmpt);

	virtual bool onMouseLeftDown(int x, int y);
	virtual bool onMouseLeftUp(int x, int y);
	virtual bool onMouseDrag(int x, int y);

	virtual bool onDraw(const d2d::Screen& scr) const;
	virtual bool clear();

	void simplify();
	void updateFromSimplified();

private:
	void clearBuffer();

private:
	class UpdateChainVisitor : public d2d::IVisitor
	{
	public:
		virtual void visit(Object* object, bool& bFetchNext);
	}; // UpdateChainVisitor

	class UpdateBufferVisitor : public d2d::IVisitor
	{
	public:
		UpdateBufferVisitor(std::map<ChainShape*, ChainShape*>& simplifyBuffer);

		virtual void visit(Object* object, bool& bFetchNext);

	private:
		std::map<ChainShape*, ChainShape*>& m_simplifyBuffer;

	}; // UpdateBufferVisitor

	class OffsetVisitor : public d2d::IVisitor
	{
	public:
		OffsetVisitor(const d2d::Vector& offset);

		virtual void visit(d2d::Object* object, bool& bFetchNext);

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