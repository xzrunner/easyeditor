#ifndef _SG_LIBRARY_PAGE_H_
#define _SG_LIBRARY_PAGE_H_



namespace sg
{

class LibraryPage : public ee::LibraryPage
{
public:
	LibraryPage(wxWindow* parent, const char* name);

	virtual bool IsHandleSymbol(ee::Symbol* symbol) const;

}; // LibraryPage 

}

#endif // _SG_LIBRARY_PAGE_H_
