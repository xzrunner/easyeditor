#include "UniversalArrangeCMPT.h"
#include "UniversalArrangeOP.h"
#include "IBody.h"
#include "BodyManager.h"
#include "SpriteSelection.h"
#include "panel_msg.h"

namespace ee
{

UniversalArrangeCMPT::UniversalArrangeCMPT(wxWindow* parent, const std::string& name, wxWindow* stage_wnd, 
										   EditPanelImpl* stage, MultiSpritesImpl* sprites_impl,
										   PropertySettingPanel* property)
	: EditCMPT(parent, name, stage)
	, m_spritesImpl(sprites_impl)
{
	m_editop = new UniversalArrangeOP(stage_wnd, stage, sprites_impl, property, this);
}

void UniversalArrangeCMPT::UpdateControlValue()
{
	SpriteSelection* selection = m_spritesImpl->GetSpriteSelection();
	if (!selection->IsEmpty())
	{
		GetPhysicsStaticVisitor visitor;
		selection->Traverse(visitor);
		switch (visitor.getType())
		{
		case GetPhysicsStaticVisitor::e_checked:
			m_physicsStaticCtrl->Set3StateValue(wxCHK_CHECKED);
			break;
		case GetPhysicsStaticVisitor::e_unchecked:
			m_physicsStaticCtrl->Set3StateValue(wxCHK_UNCHECKED);
			break;
		case GetPhysicsStaticVisitor::e_uncertain:
			m_physicsStaticCtrl->Set3StateValue(wxCHK_UNDETERMINED);
			break;
		}
	}
	else
	{
		m_physicsStaticCtrl->Set3StateValue(wxCHK_UNCHECKED);
	}
}

void UniversalArrangeCMPT::AddPhysicsEditOP(b2World* world, b2Body* ground)
{
	UniversalArrangeOP* op = static_cast<UniversalArrangeOP*>(m_editop);
	op->addPhysicsEditOP(world, ground);
}

wxSizer* UniversalArrangeCMPT::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	m_physicsStaticCtrl = new wxCheckBox(this, wxID_ANY, wxT("¹Ì¶¨"), wxDefaultPosition, wxDefaultSize, wxCHK_3STATE);
	m_physicsStaticCtrl->SetValue(false);
	Connect(m_physicsStaticCtrl->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(UniversalArrangeCMPT::onChangeStaticType));
	sizer->Add(m_physicsStaticCtrl);

	return sizer;
}

void UniversalArrangeCMPT::onChangeStaticType(wxCommandEvent& event)
{
	SpriteSelection* selection = m_spritesImpl->GetSpriteSelection();
	if (!selection->IsEmpty())
		selection->Traverse(SetPhysicsStaticVisitor(event.IsChecked()));
	SetCanvasDirtySJ::Instance()->SetDirty();
}

//////////////////////////////////////////////////////////////////////////
// class UniversalArrangeCMPT::GetPhysicsStaticVisitor
//////////////////////////////////////////////////////////////////////////

UniversalArrangeCMPT::GetPhysicsStaticVisitor::
GetPhysicsStaticVisitor()
{
	m_type = e_uncertain;
}

void UniversalArrangeCMPT::GetPhysicsStaticVisitor::
Visit(Object* object, bool& next)
{
	Sprite* sprite = static_cast<Sprite*>(object);
	const IBody* body = BodyManager::Instance()->QueryBody(sprite);
	if (!body) {
		next = true;
		return;
	}

	b2Body* b2body = body->GetBody();
	if (m_type == e_uncertain)
	{
		m_type = (b2body->GetType() == b2_staticBody) ? e_checked : e_unchecked;
		next = true;
	} else if ((m_type == e_unchecked && b2body->GetType() == b2_staticBody) 
		    || (m_type == e_checked && b2body->GetType() != b2_staticBody)) {
		m_type = e_uncertain;
		next = false;
	} else {
		next = true;
	}
}

//////////////////////////////////////////////////////////////////////////
// class UniversalArrangeCMPT::SetPhysicsStaticVisitor
//////////////////////////////////////////////////////////////////////////

UniversalArrangeCMPT::SetPhysicsStaticVisitor::
SetPhysicsStaticVisitor(bool bChecked)
{
	m_bChecked = bChecked;
}

void UniversalArrangeCMPT::SetPhysicsStaticVisitor::
Visit(Object* object, bool& next)
{
	Sprite* sprite = static_cast<Sprite*>(object);
	const IBody* body = BodyManager::Instance()->QueryBody(sprite);
	if (body) {
		body->GetBody()->SetType(m_bChecked ? b2_staticBody : b2_dynamicBody);
	}
	next = true;
}

}
