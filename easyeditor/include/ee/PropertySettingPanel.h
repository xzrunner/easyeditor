#ifndef _EASYEDITOR_PROPERTY_SETTING_PANEL_H_
#define _EASYEDITOR_PROPERTY_SETTING_PANEL_H_

#include "Observer.h"

#include <wx/wx.h>
#include <wx/propgrid/propgrid.h>

#include <set>
#include <vector>

namespace ee
{

class PropertySetting;
class EditPanelImpl;
class Sprite;
class SpriteSelection;
class Shape;
class ShapeSelection;

class PropertySettingPanel : public wxPanel, public Observer
{
public:
	PropertySettingPanel(wxWindow* parent);
	virtual ~PropertySettingPanel();

	void SetDefaultPropertySetting();
	void SetPropertySetting(PropertySetting* setting);

	wxPropertyGrid* GetPropertyGrid() { return m_pg; }

	void OnPropertyGridChange(wxPropertyGridEvent& event);
	void UpdatePropertyGrid();
	void EnablePropertyGrid(bool enable);

	const std::string& GetType() const { return m_type; }

	void SetEditPanel(EditPanelImpl* stage) { m_stage = stage; }

protected:
	virtual PropertySetting* CreateDefaultProperty() const { return NULL; }

	virtual void OnSpriteSelected(Sprite* spr, bool clear);
	virtual void OnMultiSpriteSelected(SpriteSelection* selection);

	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	void InitLayout();

	void SelectShape(Shape* shape);
	void SelectShapeSet(ShapeSelection* selection);

	void ClearSelection();

protected:
	std::string m_type;

	wxPropertyGrid* m_pg;

	PropertySetting* m_setting;

	std::set<Sprite*> m_selection;

	// todo
	EditPanelImpl* m_stage;

}; // PropertySettingPanel

}

#endif // _EASYEDITOR_PROPERTY_SETTING_PANEL_H_