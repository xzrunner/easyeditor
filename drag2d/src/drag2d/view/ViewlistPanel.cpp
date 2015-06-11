#include "ViewlistPanel.h"
#include "ViewlistList.h"

#include "dataset/ISprite.h"
#include "dataset/ISymbol.h"
#include "widgets/VerticalImageList.h"
#include "view/EditPanel.h"
#include "view/SpritePropertySetting.h"
#include "view/PropertySettingPanel.h"
#include "view/MultiSpritesImpl.h"

namespace d2d
{

ViewlistPanel::ViewlistPanel(wxWindow* parent, EditPanel* stage,
							 MultiSpritesImpl* sprites_impl /*= NULL*/, 
							 PropertySettingPanel* property /*= NULL*/)
	: wxPanel(parent, wxID_ANY)
	, m_stage(stage)
	, m_sprites_impl(sprites_impl)
	, m_property(property)
	, m_selected_idx(0)
{
	InitLayout();
}

void ViewlistPanel::Remove(ISprite* sprite)
{
	std::vector<ISprite*>::iterator itr = m_sprites.begin();
	int idx = 0;
	for (; itr != m_sprites.end(); ++itr, ++idx) {
		if (*itr == sprite) {
			break;
		}
	}
	if (itr == m_sprites.end()) {
		return;
	}

	m_list->remove(idx);
	m_sprites.erase(itr);
}

void ViewlistPanel::Insert(ISprite* sprite)
{
//	m_list->insert(const_cast<ISymbol*>(&sprite->getSymbol()));
//  m_sprites.push_back(sprite);

	m_list->insertFront(const_cast<ISymbol*>(&sprite->getSymbol()));
	m_sprites.insert(m_sprites.begin(), sprite);
}

void ViewlistPanel::Reorder(const ISprite* sprite, bool up)
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
			std::swap(m_sprites[i], m_sprites[pos]);
			m_list->swap(i, pos);
			m_list->SetSelection(pos);
		}
	}
	else
	{
		int pos = i + 1;
		if (pos < n)
		{
			std::swap(m_sprites[i], m_sprites[pos]);
			m_list->swap(i, pos);
			m_list->SetSelection(pos);
		}
	}
}

void ViewlistPanel::ReorderSelected(bool up)
{
	m_sprites_impl->resetSpriteOrder(m_sprites[m_selected_idx], up);
}

void ViewlistPanel::OnSelected(int index)
{
	m_selected_idx = index;

	if (m_property)
	{
		d2d::IPropertySetting* setting = new d2d::SpritePropertySetting(m_stage, m_sprites[index]);
		m_property->SetPropertySetting(setting);
	}

	if (m_sprites_impl)
	{
		d2d::SpriteSelection* selection = m_sprites_impl->getSpriteSelection();
		selection->Clear();
		selection->Add(m_sprites[index]);
		m_stage->Refresh();
	}
}

void ViewlistPanel::Clear()
{
	m_list->clear();
	m_sprites.clear();
}

void ViewlistPanel::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	m_list = new ViewlistList(this);
	sizer->Add(m_list, 1, wxEXPAND);

	SetSizer(sizer);
}

} // d2d