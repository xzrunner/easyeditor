#ifndef _EASYUI_UI_STAGE_PAGE_H_
#define _EASYUI_UI_STAGE_PAGE_H_

#include <drag2d.h>

namespace eui
{

class UIStagePage : public d2d::EditPanel
{
public:
	UIStagePage(wxWindow* parent, wxTopLevelWindow* frame)
		: d2d::EditPanel(parent, frame) {}

	virtual void LoadFromFile(const char* filename) = 0;
	virtual void StoreToFile(const char* filename) const = 0;

	virtual void EnablePage(bool enable) = 0;

	virtual void OnPreview() const = 0;
	virtual void OnCode() const = 0;

	void RegistSubjects(d2d::Observer* observer);
	void UnRegistSubjects(d2d::Observer* observer);

protected:
	void AddSubject(d2d::Subject* subject) {
		m_subjects.push_back(subject);
	}

private:
	std::vector<d2d::Subject*> m_subjects;

}; // UIStagePage

}

#endif // _EASYUI_UI_STAGE_PAGE_H_