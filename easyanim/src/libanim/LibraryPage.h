#ifndef _LIBANIMATION_LIBRARY_PAGE_H_
#define _LIBANIMATION_LIBRARY_PAGE_H_



namespace libanim
{

class LibraryPage : public ee::LibraryPage
{
public:
	LibraryPage(wxWindow* parent);

	virtual bool IsHandleSymbol(ee::Symbol* symbol) const;

protected:
	virtual void OnAddPress(wxCommandEvent& event);

private:
	void loadFromJsonFile(const std::string& filename);
	void loadFromLuaFile(const std::string& filename);

}; // LibraryPage

}

#endif // _LIBANIMATION_LIBRARY_PAGE_H_