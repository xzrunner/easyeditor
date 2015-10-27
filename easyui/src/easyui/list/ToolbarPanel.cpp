#include "ToolBarPanel.h"
#include "UIList.h"

namespace eui
{
namespace list
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, UIList* list, d2d::EditPanel* editpanel)
	: d2d::ToolbarPanel(parent, NULL)
	, m_list(list)
	, m_editpanel(editpanel)
{
	SetSizer(initLayout());	
}

bool ToolbarPanel::IsCurrBgLayer() const
{
	return m_choice->GetSelection() == 0;
}

void ToolbarPanel::EnableFillingBtn(bool enable)
{
	m_btn_filling->Enable(enable);
}

wxSizer* ToolbarPanel::initLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	sizer->AddSpacer(10);
	{
		wxArrayString strings;
		strings.Add("bg");
		strings.Add("item");

		m_choice = new wxRadioBox(this, wxID_ANY, wxT("Layer"), 
			wxDefaultPosition, wxDefaultSize, strings, 1, wxRA_SPECIFY_COLS);
		sizer->Add(m_choice);
	}
	sizer->AddSpacer(10);
	{
		m_btn_filling = new wxButton(this, wxID_ANY, "filling");
		Connect(m_btn_filling->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(ToolbarPanel::OnItemFilling));
		sizer->Add(m_btn_filling);
	}

	return sizer;
}

void ToolbarPanel::OnItemFilling(wxCommandEvent& event)
{
	if (m_list->items.Size() != 2) {
		return;
	}

	std::vector<d2d::ISprite*> ori_spr;
	m_list->items.Traverse(d2d::FetchAllVisitor<d2d::ISprite>(ori_spr), d2d::DT_ALL, true);

	assert(ori_spr.size() == 2);
	if (&ori_spr[0]->GetSymbol() != &ori_spr[1]->GetSymbol()) {
		return;
	}
	if (ori_spr[0]->GetPosition().x != ori_spr[1]->GetPosition().x &&
		ori_spr[0]->GetPosition().y != ori_spr[1]->GetPosition().y) {
		return;
	}
	if (ori_spr[0]->GetPosition().x == ori_spr[1]->GetPosition().x &&
		ori_spr[0]->GetPosition().y == ori_spr[1]->GetPosition().y) {
		return;
	}

//	d2d::Rect r = ori_spr[0]->GetRect();
	std::vector<d2d::ISprite*> new_spr;
	if (ori_spr[0]->GetPosition().x == ori_spr[1]->GetPosition().x) {
		float x = ori_spr[0]->GetPosition().x;
		float y_min, y_max;
		if (ori_spr[1]->GetPosition().y > ori_spr[0]->GetPosition().y) {
			y_min = ori_spr[0]->GetPosition().y;
			y_max = ori_spr[1]->GetPosition().y;
		} else {
			y_min = ori_spr[1]->GetPosition().y;
			y_max = ori_spr[0]->GetPosition().y;
		}

		float dy = y_max - y_min;
		
		float _x = x, _y = y_min - dy;
		while (true) {
			d2d::ISprite* spr = ori_spr[0]->Clone();
			spr->SetTransform(d2d::Vector(_x, _y), ori_spr[0]->GetAngle());
			if (d2d::Math::isRectContainRect(m_list->clipbox, spr->GetRect())) {
				new_spr.push_back(spr);
				_y -= dy;
			} else {
				delete spr;
				break;
			}
		}

		_y = y_max + dy;
		while (true) {
			d2d::ISprite* spr = ori_spr[0]->Clone();
			spr->SetTransform(d2d::Vector(_x, _y), ori_spr[0]->GetAngle());
			if (d2d::Math::isRectContainRect(m_list->clipbox, spr->GetRect())) {
				new_spr.push_back(spr);
				_y += dy;
			} else {
				delete spr;
				break;
			}
		}
	} else {
		float y = ori_spr[0]->GetPosition().y;
		float x_min, x_max;
		if (ori_spr[1]->GetPosition().x > ori_spr[0]->GetPosition().x) {
			x_min = ori_spr[0]->GetPosition().x;
			x_max = ori_spr[1]->GetPosition().x;
		} else {
			x_min = ori_spr[1]->GetPosition().x;
			x_max = ori_spr[0]->GetPosition().x;
		}

		float dx = x_max - x_min;

		float _x = x_min - dx, _y = y;
		while (true) {
			d2d::ISprite* spr = ori_spr[0]->Clone();
			spr->SetTransform(d2d::Vector(_x, _y), ori_spr[0]->GetAngle());
			if (d2d::Math::isRectContainRect(m_list->clipbox, spr->GetRect())) {
				new_spr.push_back(spr);
				_x -= dx;
			} else {
				delete spr;
				break;
			}
		}

		_x = x_max + dx;
		while (true) {
			d2d::ISprite* spr = ori_spr[0]->Clone();
			spr->SetTransform(d2d::Vector(_x, _y), ori_spr[0]->GetAngle());
			if (d2d::Math::isRectContainRect(m_list->clipbox, spr->GetRect())) {
				new_spr.push_back(spr);
				_x += dx;
			} else {
				delete spr;
				break;
			}
		}		
	}

	for (int i = 0; i < new_spr.size(); ++i) {
		m_list->items.Insert(new_spr[i]);
	}

	if (!new_spr.empty()) {
		m_editpanel->SetCanvasDirty();
	}
}

}
}