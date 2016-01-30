#include "UniversalArrangeCMPT.h"
#include "UniversalArrangeOP.h"
#include "IBody.h"
#include "BodyManager.h"

namespace ephysics
{

UniversalArrangeCMPT::UniversalArrangeCMPT(wxWindow* parent, const wxString& name, wxWindow* stage_wnd, 
										   d2d::EditPanelImpl* stage, d2d::MultiSpritesImpl* spritesImpl,
										   d2d::PropertySettingPanel* propertyPanel)
	: AbstractEditCMPT(parent, name, stage)
	, m_spritesImpl(spritesImpl)
{
	m_editOP = new UniversalArrangeOP(stage_wnd, stage, spritesImpl, propertyPanel, this);
}

void UniversalArrangeCMPT::updateControlValue()
{
	d2d::SpriteSelection* selection = m_spritesImpl->GetSpriteSelection();
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

void UniversalArrangeCMPT::addPhysicsEditOP(b2World* world, b2Body* ground)
{
	UniversalArrangeOP* op = static_cast<UniversalArrangeOP*>(m_editOP);
	op->addPhysicsEditOP(world, ground);
}

wxSizer* UniversalArrangeCMPT::initLayout()
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
	d2d::SpriteSelection* selection = m_spritesImpl->GetSpriteSelection();
	if (!selection->IsEmpty())
		selection->Traverse(SetPhysicsStaticVisitor(event.IsChecked()));
	d2d::SetCanvasDirtySJ::Instance()->SetDirty();
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
Visit(d2d::Object* object, bool& next)
{
	d2d::Sprite* sprite = static_cast<d2d::Sprite*>(object);
	const IBody* body = BodyManager::Instance()->QueryBody(sprite);
	if (!body) {
		next = true;
		return;
	}

	b2Body* b2body = body->getBody();
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
Visit(d2d::Object* object, bool& next)
{
	d2d::Sprite* sprite = static_cast<d2d::Sprite*>(object);
	const IBody* body = BodyManager::Instance()->QueryBody(sprite);
	if (body) {
		body->getBody()->SetType(m_bChecked ? b2_staticBody : b2_dynamicBody);
	}
	next = true;
}

}
