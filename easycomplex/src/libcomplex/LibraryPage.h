#ifndef COMPLEX_LIBRARY_PAGE_H
#define COMPLEX_LIBRARY_PAGE_H



namespace ecomplex
{

class LibraryPage : public ee::LibraryPage
{
public:
	LibraryPage(wxWindow* parent);

	virtual bool IsHandleSymbol(ee::Symbol* symbol) const;

	virtual bool LoadFromConfig();

protected:
	virtual void OnAddPress(wxCommandEvent& event);

private:
	void loadFromJsonFile(const std::string& filename);
	void loadFromLuaFile(const std::string& filename);

}; // LibraryPage

}

#endif // COMPLEX_LIBRARY_PAGE_H