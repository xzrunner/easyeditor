#ifndef EMODELING_SELECT_BODY_OP_H
#define EMODELING_SELECT_BODY_OP_H



namespace libmodeling { class Body; }

namespace emodeling
{
	class SelectBodyOP : public ee::SelectSpritesOP
	{
	public:
		SelectBodyOP(wxWindow* stage_wnd,
			ee::EditPanelImpl* stage, 
			ee::MultiSpritesImpl* spritesImpl, 
			ee::EditCMPT* callback = NULL);

		virtual bool OnKeyDown(int keyCode);
		virtual bool OnMouseMove(int x, int y);
		virtual bool OnMouseLeftDClick(int x, int y);

		virtual bool OnDraw() const;
		virtual bool Clear();

	private:
		class DrawSelectedVisitor : public ee::Visitor
		{
		public:
			virtual void Visit(ee::Object* object, bool& next);
		}; // DrawSelectedVisitor

	private:
		libmodeling::Body* m_mouseOn;

	}; // SelectBodyOP
}

#endif // EMODELING_SELECT_BODY_OP_H
