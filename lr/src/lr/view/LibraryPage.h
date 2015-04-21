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

protected:
	virtual void onAddPress(wxCommandEvent& event);

private:
	void StoreToCfgFile();

private:
	std::string m_key;

	StagePanel* m_stage;

}; // LibraryPage 

}

#endif // _LR_LIBRARY_PAGE_H_