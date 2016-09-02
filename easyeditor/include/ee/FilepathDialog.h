#ifndef _EASYEDITOR_FILEPATH_DIALOG_H_
#define _EASYEDITOR_FILEPATH_DIALOG_H_

#include "BaseDialog.h"

namespace ee
{

class Symbol;

class FilepathDialog : public BaseDialog
{
public:
	FilepathDialog(wxWindow* parent, const std::string& filepath);

	std::string GetFilepath() const;

	void SaveLastDir();

private:
	void InitLayout(const std::string& filepath);

private:
	wxTextCtrl* m_dir;
	wxTextCtrl* m_name;

	static std::string m_last_dir;

}; // FilepathDialog

}

#endif // _EASYEDITOR_FILEPATH_DIALOG_H_