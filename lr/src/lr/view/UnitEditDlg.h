#ifndef _LR_UNIT_EDIT_DIALOG_H_
#define _LR_UNIT_EDIT_DIALOG_H_

#include <drag2d.h>

#include "dataset/UnitCfg.h"
#include "dataset/UnitInfo.h"

namespace lr
{

class UnitEditDlg : public d2d::BaseDialog
{
public:
	UnitEditDlg(wxWindow* parent, d2d::ISprite* spr, 
		const std::vector<std::string>& path_names);

	std::string ToString() const;

private:
	void InitLayout();

	wxChoice* CreateChoiceCtrl(const std::vector<UnitCfg::Item>& items, 
		const UnitInfo& info, const std::string& key);

	wxCheckBox* CreateCheckCtrl(const UnitInfo& info, const std::string& key, 
		const std::string& title);

	static void SetValue(UnitInfo& info, const std::string& key, wxCheckBox* ctrl);

private:
	d2d::ISprite* m_spr;

	const std::vector<std::string>& m_path_names;

	wxTextCtrl* m_name_ctrl;

	wxChoice* m_camp_ctrl;

	wxChoice* m_type_ctrl;

	wxChoice* m_squad_ctrl;

	wxCheckBox* m_leader_ctrl;

	wxChoice* m_dir_ctrl;

	wxCheckBox* m_scene_ai_ctrl;

	wxCheckBox* m_god_sight_ctrl;

	wxChoice* m_patrol_ctrl;

}; // UnitEditDlg

}

#endif // _LR_UNIT_EDIT_DIALOG_H_