#ifndef _EASYPARTICLE2D_TOOLBAR_PANEL_H_
#define _EASYPARTICLE2D_TOOLBAR_PANEL_H_

#include <drag2d.h>

namespace eparticle2d
{

class ParticleSystem;
class ISliderCtrl;

class ToolbarPanel : public d2d::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent, d2d::LibraryPanel* library,
		d2d::EditPanel* stage, ParticleSystem* ps);
	virtual ~ToolbarPanel();

	void Load(const Json::Value& val);
	void Store(Json::Value& val) const;

protected:
	virtual wxSizer* initLayout();

private:
	wxSizer* InitDefaultLayout();
	wxSizer* InitSpecialLayout();

	void InitPSValue();

private:
	class DropTarget : public wxTextDropTarget
	{
	public:
		DropTarget(d2d::LibraryPanel* library, ParticleSystem* ps);

		virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& data);

	private:
		d2d::LibraryPanel* m_library;

		ParticleSystem* m_ps;

	}; // DropTarget

private:
	ParticleSystem* m_ps;

	std::vector<ISliderCtrl*> m_sliders;

}; // ToolbarPanel

}

#endif // _EASYPARTICLE2D_TOOLBAR_PANEL_H_