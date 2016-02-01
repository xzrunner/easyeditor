#ifndef _EPHYSICS_UNIVERSAL_ARRANGE_CMPT_H_
#define _EPHYSICS_UNIVERSAL_ARRANGE_CMPT_H_


#include <Box2D/Box2D.h>

namespace ephysics
{

class UniversalArrangeCMPT : public ee::EditCMPT
{
public:
	UniversalArrangeCMPT(wxWindow* parent, const wxString& name, wxWindow* stage_wnd, 
		ee::EditPanelImpl* stage, ee::MultiSpritesImpl* spritesImpl,
		ee::PropertySettingPanel* property);

	virtual void UpdateControlValue();

	void addPhysicsEditOP(b2World* world, b2Body* ground);

protected:
	virtual wxSizer* InitLayout();

private:
	class GetPhysicsStaticVisitor : public ee::Visitor
	{
	public:
		enum TYPE
		{
			e_checked,
			e_unchecked,
			e_uncertain
		};

	public:
		GetPhysicsStaticVisitor();
		virtual void Visit(ee::Object* object, bool& next);
		TYPE getType() const { return m_type; }

	private:
		TYPE m_type;	// 0 all open, 1 all close, 2 uncertain

	}; // GetPhysicsStaticVisitor

	class SetPhysicsStaticVisitor : public ee::Visitor
	{
	public:
		SetPhysicsStaticVisitor(bool bChecked);
		virtual void Visit(ee::Object* object, bool& next);

	private:
		bool m_bChecked;

	}; // SetPhysicsStaticVisitor

private:
	void onChangeStaticType(wxCommandEvent& event);

private:
	ee::MultiSpritesImpl* m_spritesImpl;

	wxCheckBox* m_physicsStaticCtrl;
	
}; // UniversalArrangeCMPT

}

#endif // _EPHYSICS_UNIVERSAL_ARRANGE_CMPT_H_