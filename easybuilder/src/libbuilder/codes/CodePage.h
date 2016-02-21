#ifndef _EASYBUILDER_CODE_PAGE_H_
#define _EASYBUILDER_CODE_PAGE_H_

#include <wx/stc/stc.h>

struct LanguageInfo;

namespace ebuilder
{

class CodePage : public wxStyledTextCtrl
{
public:
	CodePage(wxWindow* parent, const std::string& name);
	virtual ~CodePage() {}

	const std::string& getName() const;

protected:
	//! language/lexer
	wxString DeterminePrefs (const wxString& filename);
	bool InitializePrefs (const std::string& filename);

private:
	std::string m_name;

	// lanugage properties
	const LanguageInfo* m_language;

	// margin variables
	int m_LineNrID;
	int m_LineNrMargin;
	int m_FoldingID;
	int m_FoldingMargin;
	int m_DividerID;

}; // CodePage

}

#endif // _EASYBUILDER_CODE_PAGE_H_
