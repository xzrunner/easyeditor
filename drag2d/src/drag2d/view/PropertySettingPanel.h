#ifndef _DRAG2D_PROPERTY_SETTING_PANEL_H_
#define _DRAG2D_PROPERTY_SETTING_PANEL_H_

#include <wx/wx.h>
#include <wx/propgrid/propgrid.h>

#include "ISpriteViewPanel.h"
#include "IShapeViewPanel.h"

namespace d2d
{

class IPropertySetting;

class PropertySettingPanel : public wxPanel, public ISpriteViewPanel, 
	public IShapeViewPanel
{
public:
	PropertySettingPanel(wxWindow* parent);
	virtual ~PropertySettingPanel();

	//
	//	interface ISpriteViewPanel
	//
	virtual void SelectSprite(ISprite* spr);
	virtual void SelectMultiSprites(SpriteSelection* selection);
	virtual void ReorderSprite(ISprite* spr, bool up) {}
	virtual void InsertSprite(ISprite* spr) {}
	virtual void RemoveSprite(ISprite* spr) {}

	//
	//	interface IShapeViewPanel
	//
	virtual void SelectShape(IShape* shape);
	virtual void SelectMultiShapes(ShapeSelection* selection);

	void SetPropertySetting(IPropertySetting* setting);

	wxPropertyGrid* GetPropertyGrid() { return m_pg; }

	void OnPropertyGridChange(wxPropertyGridEvent& event);
	void UpdatePropertyGrid();
	void EnablePropertyGrid(bool enable);

	const std::string& GetType() const { return m_type; }

	void SetEditPanel(EditPanel* stage) { m_stage = stage; }

protected:
	virtual IPropertySetting* CreateDefaultProperty() const { return NULL; }

private:
	void InitLayout();

protected:
	std::string m_type;

	wxPropertyGrid* m_pg;

	IPropertySetting* m_setting;

	// todo
	EditPanel* m_stage;

}; // PropertySettingPanel

}

#endif // _DRAG2D_PROPERTY_SETTING_PANEL_H_