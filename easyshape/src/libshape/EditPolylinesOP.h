#ifndef _EASYSHAPE_EDIT_POLYLINES_OP_H_
#define _EASYSHAPE_EDIT_POLYLINES_OP_H_

#include <ee/SelectShapesOP.h>
#include <ee/Shape.h>

#include <map>

namespace eshape
{

class ChainShape;
class EditPolylinesCMPT;

class EditPolylinesOP : public ee::SelectShapesOP
{
public:
	EditPolylinesOP(wxWindow* wnd, ee::EditPanelImpl* stage, ee::MultiShapesImpl* shapes_impl,
		EditPolylinesCMPT* cmpt);

	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseLeftUp(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;

	virtual bool OnDraw() const override;
	virtual bool Clear() override;

	void simplify();
	void updateFromSimplified();

private:
	void clearBuffer();

private:
	class UpdateBufferVisitor : public ee::RefVisitor<ee::Shape>
	{
	public:
		UpdateBufferVisitor(std::map<std::shared_ptr<ChainShape>, std::shared_ptr<ChainShape>>& simplifyBuffer);

		virtual void Visit(const ee::ShapePtr& shape, bool& next) override;

	private:
		std::map<std::shared_ptr<ChainShape>, std::shared_ptr<ChainShape>>& m_simplify_buffer;

	}; // UpdateBufferVisitor

	class OffsetVisitor : public ee::RefVisitor<ee::Shape>
	{
	public:
		OffsetVisitor(const sm::vec2& offset);

		virtual void Visit(const ee::ShapePtr& shape, bool& next) override;

	private:
		const sm::vec2& m_offset;

	}; // OffsetVisitor

private:
	EditPolylinesCMPT* m_cmpt;

	std::map<std::shared_ptr<ChainShape>, std::shared_ptr<ChainShape>> m_simplify_buffer;

	sm::vec2 m_last_pos;

	bool m_is_dirty;

}; // EditPolylinesOP

}

#endif // _EASYSHAPE_EDIT_POLYLINES_OP_H_