#ifndef _EPHYSICS_UNIVERSAL_ARRANGE_CMPT_H_
#define _EPHYSICS_UNIVERSAL_ARRANGE_CMPT_H_

#include <drag2d.h>
#include <Box2D/Box2D.h>

namespace ephysics
{

class UniversalArrangeCMPT : public d2d::AbstractEditCMPT
{
public:
	UniversalArrangeCMPT(wxWindow* parent, const wxString& name, wxWindow* stage_wnd, 
		d2d::EditPanelImpl* stage, d2d::MultiSpritesImpl* spritesImpl,
		d2d::PropertySettingPanel* propertyPanel);

	virtual void updateControlValue();

	void addPhysicsEditOP(b2World* world, b2Body* ground);

protected:
	virtual wxSizer* initLayout();

private:
	class GetPhysicsStaticVisitor : public d2d::IVisitor
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
		virtual void Visit(d2d::Object* object, bool& bFetchNext);
		TYPE getType() const { return m_type; }

	private:
		TYPE m_type;	// 0 all open, 1 all close, 2 uncertain

	}; // GetPhysicsStaticVisitor

	class SetPhysicsStaticVisitor : public d2d::IVisitor
	{
	public:
		SetPhysicsStaticVisitor(bool bChecked);
		virtual void Visit(d2d::Object* object, bool& bFetchNext);

	private:
		bool m_bChecked;

	}; // SetPhysicsStaticVisitor

private:
	void onChangeStaticType(wxCommandEvent& event);

private:
	d2d::MultiSpritesImpl* m_spritesImpl;

	wxCheckBox* m_physicsStaticCtrl;
	
}; // UniversalArrangeCMPT

}

#endif // _EPHYSICS_UNIVERSAL_ARRANGE_CMPT_H_