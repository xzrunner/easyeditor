#ifndef _DRAG2D_PROPERTY_SETTING_PANEL_H_
#define _DRAG2D_PROPERTY_SETTING_PANEL_H_

#include <wx/wx.h>
#include <wx/propgrid/propgrid.h>
#include <set>
#include <vector>

#include "message/Observer.h"

namespace d2d
{

class IPropertySetting;
class EditPanelImpl;
class ISprite;
class SpriteSelection;
class IShape;
class ShapeSelection;

class PropertySettingPanel : public wxPanel, public Observer
{
public:
	PropertySettingPanel(wxWindow* parent);
	virtual ~PropertySettingPanel();

	void SetPropertySetting(IPropertySetting* setting);

	wxPropertyGrid* GetPropertyGrid() { return m_pg; }

	void OnPropertyGridChange(wxPropertyGridEvent& event);
	void UpdatePropertyGrid();
	void EnablePropertyGrid(bool enable);

	const std::string& GetType() const { return m_type; }

	void SetEditPanel(EditPanelImpl* stage) { m_stage = stage; }

protected:
	virtual IPropertySetting* CreateDefaultProperty() const { return NULL; }

	virtual void OnSpriteSelected(ISprite* spr, bool clear);
	virtual void OnMultiSpriteSelected(SpriteSelection* selection);

	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	void InitLayout();

	void SelectShape(IShape* shape);
	void SelectShapeSet(ShapeSelection* selection);

	void ClearSelection();

protected:
	std::string m_type;

	wxPropertyGrid* m_pg;

	IPropertySetting* m_setting;

	std::set<ISprite*> m_selection;

	// todo
	EditPanelImpl* m_stage;

}; // PropertySettingPanel

}

#endif // _DRAG2D_PROPERTY_SETTING_PANEL_H_