#ifndef _EASYANIM_FRAME_H_
#define _EASYANIM_FRAME_H_

#include <drag2d.h>

namespace eanim
{

class Frame : public d2d::Frame
{
public:
	Frame(const wxString& title, const wxString& filetag);

protected:
	virtual void onSaveAs(wxCommandEvent& event);

private:
	void OnPreview(wxCommandEvent& event);
	void OnSetBackground(wxCommandEvent& event);
	void OnCodeSetting(wxCommandEvent& event);
	void OnCodeLove2d(wxCommandEvent& event);

	void SaveAsPNG(const std::string& filepath) const;
	void SaveAsJson(const std::string& filepath) const;

private:
	enum
	{
		ID_PREVIEW = 2000,
		ID_BACKGROUND,
		ID_CODESETTING,
		ID_LOVE2D
	};

private:
	DECLARE_EVENT_TABLE()

}; // Frame

}

#endif // _EASYANIM_FRAME_H_