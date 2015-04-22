#ifndef _LR_LIBRARY_PAGE_H_
#define _LR_LIBRARY_PAGE_H_

#include <drag2d.h>

namespace lr
{

class StagePanel;

class LibraryPage : public d2d::ILibraryPage
{
public:
	LibraryPage(wxWindow* parent, const char* name, 
		const std::string& key, StagePanel* stage);
	virtual ~LibraryPage();

	virtual bool isHandleSymbol(d2d::ISymbol* symbol) const;

	void LoadFromFile(const Json::Value& value, const std::string& dir);
	void StoreToFile(Json::Value& value, const std::string& dir) const;

protected:
	virtual void InitLayoutExtend(wxSizer* sizer);

	virtual void onAddPress(wxCommandEvent& event);

private:
	void OnChangeVisible(wxCommandEvent& event);

	void VisibleAllSprites(bool visible);

private:
	std::string m_key;

	StagePanel* m_stage;

	wxCheckBox* m_visible_ctrl;
	bool m_visible;

}; // LibraryPage 

}

#endif // _LR_LIBRARY_PAGE_H_