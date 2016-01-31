
#include "BehaviorsPage.h"

#include "behaviors/BehaviorPanel.h"
#include "behaviors/BehaviorFactory.h"
#include "behaviors/BehaviorItem.h"

using namespace ebuilder;

BehaviorsPage::BehaviorsPage(wxWindow* parent)
	: ee::ILibraryPage(parent, wxT("Behaviors"))
{
	initLayout();
	initBehaviors();
}

bool BehaviorsPage::isHandleSymbol(ee::ISymbol* symbol) const
{
	return false;
}

void BehaviorsPage::clear()
{
}

void BehaviorsPage::initLayout(bool draggable /*= true*/)
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	if (!m_list)
		m_list = new ee::LibraryList(this, wxT("behaviors"), /*false*/true);
	sizer->Add(m_list, 1, wxEXPAND);

	SetSizer(sizer);
}

void BehaviorsPage::onAddPress(wxCommandEvent& event)
{

}

void BehaviorsPage::initBehaviors()
{
	m_list->insert(new BehaviorItem(CHANGE_IMAGE_NAME, e_ChangeImage));
	m_list->insert(new BehaviorItem(CHANGE_SCENE_NAME, e_ChangeScene));
	m_list->insert(new BehaviorItem(SCALE_SIZE_NAME, e_ScaleSize));
	m_list->insert(new BehaviorItem(MOUSE_BUTTON_NAME, e_MouseButton));
	m_list->insert(new BehaviorItem(MOVE_NAME, e_Move));
	m_list->insert(new BehaviorItem(ROTATE_NAME, e_Rotate));
}
