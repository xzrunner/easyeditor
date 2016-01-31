#ifndef _EASYUI_WRAPPER_TOOLBAR_PANEL_H_
#define _EASYUI_WRAPPER_TOOLBAR_PANEL_H_



namespace eui
{
namespace wrapper
{

class ToolbarPanel : public ee::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent);

	std::string GetType() const;
	void SetType(const std::string& type);

	std::string GetTag() const;
	void SetTag(const std::string& tag);

protected:
	virtual wxSizer* InitLayout();

private:
	wxTextCtrl* m_type_text;
	wxTextCtrl* m_tag_text;

}; // ToolbarPanel

}
}

#endif // _EASYUI_WRAPPER_TOOLBAR_PANEL_H_