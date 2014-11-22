#include "UniversalArrangeCMPT.h"

#include "dataset/IBody.h"
#include "operator/UniversalArrangeOP.h"
#include "view/MultiSpritesImpl.h"

namespace d2d
{

UniversalArrangeCMPT::UniversalArrangeCMPT(wxWindow* parent, const wxString& name, 
										   EditPanel* editPanel, MultiSpritesImpl* spritesImpl,
										   PropertySettingPanel* propertyPanel)
	: AbstractEditCMPT(parent, name, editPanel)
	, m_spritesImpl(spritesImpl)
{
	m_editOP = new UniversalArrangeOP(editPanel, spritesImpl, propertyPanel, this);
}

void UniversalArrangeCMPT::updateControlValue()
{
	SpriteSelection* selection = m_spritesImpl->getSpriteSelection();
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
	SpriteSelection* selection = m_spritesImpl->getSpriteSelection();
	if (!selection->IsEmpty())
		selection->Traverse(SetPhysicsStaticVisitor(event.IsChecked()));
	m_editPanel->Refresh();
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
visit(Object* object, bool& bFetchNext)
{
	ISprite* sprite = static_cast<ISprite*>(object);
	if (!sprite->getBody())
	{
		bFetchNext = true;
		return;
	}

	b2Body* body = sprite->getBody()->getBody();
	if (m_type == e_uncertain)
	{
		m_type = (body->GetType() == b2_staticBody) ? e_checked : e_unchecked;
		bFetchNext = true;
	}
	else if ((m_type == e_unchecked && body->GetType() == b2_staticBody)
		|| (m_type == e_checked && body->GetType() != b2_staticBody))
	{
		m_type = e_uncertain;
		bFetchNext = false;
	}
	else
	{
		bFetchNext = true;
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
visit(Object* object, bool& bFetchNext)
{
	ISprite* sprite = static_cast<ISprite*>(object);
	if (sprite->getBody())
		sprite->getBody()->getBody()->SetType(m_bChecked ? b2_staticBody : b2_dynamicBody);

	bFetchNext = true;
}

} // d2d