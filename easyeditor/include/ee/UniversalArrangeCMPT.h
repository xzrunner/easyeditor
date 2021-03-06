#ifndef _EASYEDITOR_UNIVERSAL_ARRANGE_CMPT_H_
#define _EASYEDITOR_UNIVERSAL_ARRANGE_CMPT_H_

#include "EditCMPT.h"
#include "Visitor.h"
#include "Sprite.h"

#include <Box2D/Box2D.h>

namespace ee
{

class MultiSpritesImpl;
class PropertySettingPanel;

class UniversalArrangeCMPT : public EditCMPT
{
public:
	UniversalArrangeCMPT(wxWindow* parent, const std::string& name, wxWindow* stage_wnd, 
		EditPanelImpl* stage, MultiSpritesImpl* sprites_impl,
		PropertySettingPanel* property);

	virtual void UpdateControlValue() override;

	void AddPhysicsEditOP(b2World* world, b2Body* ground);

protected:
	virtual wxSizer* InitLayout() override;

private:
	class GetPhysicsStaticVisitor : public RefVisitor<Sprite>
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
		virtual void Visit(const SprPtr& spr, bool& next) override;
		TYPE getType() const { return m_type; }

	private:
		TYPE m_type;	// 0 all open, 1 all close, 2 uncertain

	}; // GetPhysicsStaticVisitor

	class SetPhysicsStaticVisitor : public RefVisitor<Sprite>
	{
	public:
		SetPhysicsStaticVisitor(bool bChecked);
		virtual void Visit(const SprPtr& spr, bool& next) override;

	private:
		bool m_bChecked;

	}; // SetPhysicsStaticVisitor

private:
	void onChangeStaticType(wxCommandEvent& event);

private:
	MultiSpritesImpl* m_spritesImpl;

	wxCheckBox* m_physicsStaticCtrl;
	
}; // UniversalArrangeCMPT

}

#endif // _EASYEDITOR_UNIVERSAL_ARRANGE_CMPT_H_