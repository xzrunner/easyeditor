#ifndef _DRAG2D_PROPERTY_SETTING_PANEL_H_
#define _DRAG2D_PROPERTY_SETTING_PANEL_H_

#include <wx/wx.h>
#include <wx/propgrid/propgrid.h>
#include <set>

#include "ISpriteViewPanel.h"
#include "IShapeViewPanel.h"

#include "message/Observer.h"

namespace d2d
{

class IPropertySetting;
class EditPanelImpl;

class PropertySettingPanel : public wxPanel, public ISpriteViewPanel, public IShapeViewPanel, public Observer
{
public:
	PropertySettingPanel(wxWindow* parent);
	virtual ~PropertySettingPanel();

	//
	//	interface ISpriteViewPanel
	//
	virtual bool ReorderSprite(ISprite* spr, bool up) { return false; }
	virtual bool InsertSprite(ISprite* spr, int idx = -1);
	virtual bool RemoveSprite(ISprite* spr);
	virtual bool ClearAllSprite();

	//
	//	interface IShapeViewPanel
	//
	virtual void SelectShape(IShape* shape);
	virtual void SelectMultiShapes(ShapeSelection* selection);
	virtual void RemoveShape(IShape* shape);

	//
	//	interface Observer
	//
	virtual void Notify(int sj_id, void* ud);

	void SetPropertySetting(IPropertySetting* setting);

	wxPropertyGrid* GetPropertyGrid() { return m_pg; }

	void OnPropertyGridChange(wxPropertyGridEvent& event);
	void UpdatePropertyGrid();
	void EnablePropertyGrid(bool enable);

	const std::string& GetType() const { return m_type; }

	void SetEditPanel(EditPanelImpl* stage) { m_stage = stage; }

protected:
	virtual IPropertySetting* CreateDefaultProperty() const { return NULL; }

	virtual void OnSpriteSelected(d2d::ISprite* spr, bool clear);
	virtual void OnMultiSpriteSelected(SpriteSelection* selection);

private:
	void InitLayout();

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