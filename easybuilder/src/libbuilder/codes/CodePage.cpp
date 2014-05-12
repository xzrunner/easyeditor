
#include "CodePage.h"
#include "defsext.h"
#include "prefs.h"

using namespace ebuilder;

CodePage::CodePage(wxWindow* parent, const wxString& name)
	: wxStyledTextCtrl(parent), m_name(name) 
{
	m_LineNrID = 0;
	m_DividerID = 1;
	m_FoldingID = 2;

	// initialize language
	m_language = NULL;

	// default font for all styles
	SetWrapMode (wxSTC_WRAP_WORD/*wxSTC_WRAP_NONE*/);
	wxFont font (10, wxMODERN, wxNORMAL, wxNORMAL);
	StyleSetFont (wxSTC_STYLE_DEFAULT, font);
	StyleSetForeground (wxSTC_STYLE_DEFAULT, *wxBLACK);
	StyleSetBackground (wxSTC_STYLE_DEFAULT, *wxWHITE);
	StyleSetForeground (wxSTC_STYLE_LINENUMBER, wxColour (wxT("DARK GREY")));
	StyleSetBackground (wxSTC_STYLE_LINENUMBER, *wxWHITE);
	StyleSetForeground(wxSTC_STYLE_INDENTGUIDE, wxColour (wxT("DARK GREY")));

	// set visibility
 	SetVisiblePolicy (wxSTC_VISIBLE_STRICT|wxSTC_VISIBLE_SLOP, 1);
 	SetXCaretPolicy (wxSTC_CARET_EVEN|wxSTC_VISIBLE_STRICT|wxSTC_CARET_SLOP, 1);
 	SetYCaretPolicy (wxSTC_CARET_EVEN|wxSTC_VISIBLE_STRICT|wxSTC_CARET_SLOP, 1);

	// markers
	MarkerDefine (wxSTC_MARKNUM_FOLDER,        wxSTC_MARK_DOTDOTDOT, wxT("BLACK"), wxT("BLACK"));
	MarkerDefine (wxSTC_MARKNUM_FOLDEROPEN,    wxSTC_MARK_ARROWDOWN, wxT("BLACK"), wxT("BLACK"));
	MarkerDefine (wxSTC_MARKNUM_FOLDERSUB,     wxSTC_MARK_EMPTY,     wxT("BLACK"), wxT("BLACK"));
	MarkerDefine (wxSTC_MARKNUM_FOLDEREND,     wxSTC_MARK_DOTDOTDOT, wxT("BLACK"), wxT("WHITE"));
	MarkerDefine (wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_ARROWDOWN, wxT("BLACK"), wxT("WHITE"));
	MarkerDefine (wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY,     wxT("BLACK"), wxT("BLACK"));
	MarkerDefine (wxSTC_MARKNUM_FOLDERTAIL,    wxSTC_MARK_EMPTY,     wxT("BLACK"), wxT("BLACK"));

	// miscelaneous
	m_LineNrMargin = TextWidth (wxSTC_STYLE_LINENUMBER, wxT("_999999"));
	m_FoldingMargin = 16;
	CmdKeyClear (wxSTC_KEY_TAB, 0); // this is done by the menu accelerator key
	SetLayoutCache (wxSTC_CACHE_PAGE);
}

const wxString& CodePage::getName() const 
{ 
	return m_name; 
}

wxString CodePage::DeterminePrefs (const wxString &filename) 
{
	LanguageInfo const* curInfo;

	// determine language from filepatterns
	int languageNr;
	for (languageNr = 0; languageNr < g_LanguagePrefsSize; languageNr++) {
		curInfo = &g_LanguagePrefs [languageNr];
		wxString filepattern = curInfo->filepattern;
		filepattern.Lower();
		while (!filepattern.empty()) {
			wxString cur = filepattern.BeforeFirst (';');
			if ((cur == filename) ||
				(cur == (filename.BeforeLast ('.') + wxT(".*"))) ||
				(cur == (wxT("*.") + filename.AfterLast ('.')))) {
					return curInfo->name;
			}
			filepattern = filepattern.AfterFirst (';');
		}
	}
	return wxEmptyString;
}

bool CodePage::InitializePrefs (const wxString &name) 
{
	// initialize styles
	StyleClearAll();
	LanguageInfo const* curInfo = NULL;

	// determine language
	bool found = false;
	int languageNr;
	for (languageNr = 0; languageNr < g_LanguagePrefsSize; languageNr++) {
		curInfo = &g_LanguagePrefs [languageNr];
		if (curInfo->name == name) {
			found = true;
			break;
		}
	}
	if (!found) return false;

	// set lexer and language
	SetLexer (curInfo->lexer);
	m_language = curInfo;

	// set margin for line numbers
	SetMarginType (m_LineNrID, wxSTC_MARGIN_NUMBER);
	StyleSetForeground (wxSTC_STYLE_LINENUMBER, wxColour (wxT("DARK GREY")));
	StyleSetBackground (wxSTC_STYLE_LINENUMBER, *wxWHITE);
	SetMarginWidth (m_LineNrID, 0); // start out not visible

	// default fonts for all styles!
	int Nr;
	for (Nr = 0; Nr < wxSTC_STYLE_LASTPREDEFINED; Nr++) {
		wxFont font (10, wxMODERN, wxNORMAL, wxNORMAL);
		StyleSetFont (Nr, font);
	}

	// set common styles
	StyleSetForeground (wxSTC_STYLE_DEFAULT, wxColour (wxT("DARK GREY")));
	StyleSetForeground (wxSTC_STYLE_INDENTGUIDE, wxColour (wxT("DARK GREY")));

	// line numbers
	if (g_CommonPrefs.lineNumberEnable)
		SetMarginWidth (m_LineNrID, GetMarginWidth (m_LineNrID) == 0? m_LineNrMargin: 0);

	// initialize settings
	if (g_CommonPrefs.syntaxEnable) {
		int keywordnr = 0;
		for (Nr = 0; Nr < STYLE_TYPES_COUNT; Nr++) {
			if (curInfo->styles[Nr].type == -1) continue;
			const StyleInfo &curType = g_StylePrefs [curInfo->styles[Nr].type];
			wxFont font (curType.fontsize, wxMODERN, wxNORMAL, wxNORMAL, false,
				curType.fontname);
			StyleSetFont (Nr, font);
			if (curType.foreground) {
				StyleSetForeground (Nr, wxColour (curType.foreground));
			}
			if (curType.background) {
				StyleSetBackground (Nr, wxColour (curType.background));
			}
			StyleSetBold (Nr, (curType.fontstyle & mySTC_STYLE_BOLD) > 0);
			StyleSetItalic (Nr, (curType.fontstyle & mySTC_STYLE_ITALIC) > 0);
			StyleSetUnderline (Nr, (curType.fontstyle & mySTC_STYLE_UNDERL) > 0);
			StyleSetVisible (Nr, (curType.fontstyle & mySTC_STYLE_HIDDEN) == 0);
			StyleSetCase (Nr, curType.lettercase);
			const char *pwords = curInfo->styles[Nr].words;
			if (pwords) {
				SetKeyWords (keywordnr, pwords);
				keywordnr += 1;
			}
		}
	}

	// set margin as unused
	SetMarginType (m_DividerID, wxSTC_MARGIN_SYMBOL);
	SetMarginWidth (m_DividerID, 0);
	SetMarginSensitive (m_DividerID, false);

	// folding
	SetMarginType (m_FoldingID, wxSTC_MARGIN_SYMBOL);
	SetMarginMask (m_FoldingID, wxSTC_MASK_FOLDERS);
	StyleSetBackground (m_FoldingID, *wxWHITE);
	SetMarginWidth (m_FoldingID, 0);
	SetMarginSensitive (m_FoldingID, false);
 	if (g_CommonPrefs.foldEnable) {
  		SetMarginWidth (m_FoldingID, curInfo->folds != 0? m_FoldingMargin: 0);
  		SetMarginSensitive (m_FoldingID, curInfo->folds != 0);
  		SetProperty (wxT("fold"), curInfo->folds != 0? wxT("1"): wxT("0"));
   		SetProperty (wxT("fold.comment"),
   			(curInfo->folds & mySTC_FOLD_COMMENT) > 0? wxT("1"): wxT("0"));
   		SetProperty (wxT("fold.compact"),
   			(curInfo->folds & mySTC_FOLD_COMPACT) > 0? wxT("1"): wxT("0"));
   		SetProperty (wxT("fold.preprocessor"),
   			(curInfo->folds & mySTC_FOLD_PREPROC) > 0? wxT("1"): wxT("0"));
   		SetProperty (wxT("fold.html"),
   			(curInfo->folds & mySTC_FOLD_HTML) > 0? wxT("1"): wxT("0"));
  		SetProperty (wxT("fold.html.preprocessor"),
  			(curInfo->folds & mySTC_FOLD_HTMLPREP) > 0? wxT("1"): wxT("0"));
   		SetProperty (wxT("fold.comment.python"),
   			(curInfo->folds & mySTC_FOLD_COMMENTPY) > 0? wxT("1"): wxT("0"));
   		SetProperty (wxT("fold.quotes.python"),
   			(curInfo->folds & mySTC_FOLD_QUOTESPY) > 0? wxT("1"): wxT("0"));
 	}
	SetFoldFlags (wxSTC_FOLDFLAG_LINEBEFORE_CONTRACTED |
		wxSTC_FOLDFLAG_LINEAFTER_CONTRACTED);

	// set spaces and indention
	SetTabWidth (4);
	SetUseTabs (false);
	SetTabIndents (true);
	SetBackSpaceUnIndents (true);
	SetIndent (g_CommonPrefs.indentEnable? 4: 0);

	// others
	SetViewEOL (g_CommonPrefs.displayEOLEnable);
	SetIndentationGuides (g_CommonPrefs.indentGuideEnable);
	SetEdgeColumn (80);
	SetEdgeMode (g_CommonPrefs.longLineOnEnable? wxSTC_EDGE_LINE: wxSTC_EDGE_NONE);
	SetViewWhiteSpace (g_CommonPrefs.whiteSpaceEnable?
wxSTC_WS_VISIBLEALWAYS: wxSTC_WS_INVISIBLE);
	SetOvertype (g_CommonPrefs.overTypeInitial);
	SetReadOnly (g_CommonPrefs.readOnlyInitial);
	SetWrapMode (g_CommonPrefs.wrapModeInitial?
wxSTC_WRAP_WORD: wxSTC_WRAP_NONE);

	return true;
}
