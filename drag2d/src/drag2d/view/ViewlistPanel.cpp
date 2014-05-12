#include "ViewlistPanel.h"
#include "ViewlistList.h"

#include "dataset/ISprite.h"
#include "dataset/ISymbol.h"
#include "dataset/SelectionSet.h"
#include "widgets/VerticalImageList.h"
#include "view/EditPanel.h"
#include "view/SpritePropertySetting.h"
#include "view/PropertySettingPanel.h"
#include "view/MultiSpritesImpl.h"

namespace d2d
{

ViewlistPanel::ViewlistPanel(wxWindow* parent, EditPanel* editPanel,
							 MultiSpritesImpl* stage /*= NULL*/, 
							 PropertySettingPanel* property /*= NULL*/)
	: wxPanel(parent, wxID_ANY)
	, m_editPanel(editPanel)
	, m_stage(stage)
	, m_property(property)
{
	initLayout();
}

void ViewlistPanel::remove(ISprite* sprite)
{
	size_t i = 0;
	for (size_t n = m_sprites.size(); i < n; ++i)
		if (m_sprites[i] == sprite)
			break;
	assert(i != m_sprites.size());

	m_list->remove(i);
	m_sprites.erase(m_sprites.begin() + i);
}

void ViewlistPanel::insert(ISprite* sprite)
{
//	m_list->insert(const_cast<ISymbol*>(&sprite->getSymbol()));
//  m_sprites.push_back(sprite);

	m_list->insertFront(const_cast<ISymbol*>(&sprite->getSymbol()));
	m_sprites.insert(m_sprites.begin(), sprite);
}

void ViewlistPanel::reorder(const ISprite* sprite, bool up)
{
	int i = 0;
	int n = m_sprites.size();
	for ( ; i < n; ++i)
		if (m_sprites[i] == sprite)
			break;
	assert(i != n);

	if (up)
	{
		int pos = i - 1;
		if (pos >= 0)
		{
			swap(i, pos);
			m_list->swap(i, pos);
		}
	}
	else
	{
		int pos = i + 1;
		if (pos < n)
		{
			swap(i, pos);
			m_list->swap(i, pos);
		}
	}
}

void ViewlistPanel::onSelected(int index)
{
	if (m_property)
	{
		d2d::IPropertySetting* setting = new d2d::SpritePropertySetting(m_editPanel, m_sprites[index]);
		m_property->setPropertySetting(setting);
	}

	if (m_stage)
	{
		d2d::SpriteSelection* selection = m_stage->getSpriteSelection();
		selection->clear();
		selection->insert(m_sprites[index]);
		m_editPanel->Refresh();
	}
}

void ViewlistPanel::clear()
{
	m_list->clear();
	m_sprites.clear();
}

void ViewlistPanel::initLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	m_list = new ViewlistList(this);
	sizer->Add(m_list, 1, wxEXPAND);

	SetSizer(sizer);
}

void ViewlistPanel::swap(int i0, int i1)
{
	if (i0 < 0 || i0 >= m_sprites.size() ||
		i1 < 0 || i1 >= m_sprites.size())
		return;

	ISprite* tmp = m_sprites[i0];
	m_sprites[i0] = m_sprites[i1];
	m_sprites[i1] = tmp;
}

} // d2d