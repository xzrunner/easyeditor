#ifndef _LR_MAIN_DIALOG_H_
#define _LR_MAIN_DIALOG_H_

#include <drag2d.h>

namespace lr
{
namespace preview
{

class MainDialog : public wxDialog
{
public:
	MainDialog(wxWindow* parent, int width, int height, 
		const std::vector<const d2d::Sprite*>& sprites);

private:
	void InitLayout();

private:
	std::vector<const d2d::Sprite*> m_sprites;

	d2d::PlayControl m_control;

}; // MainDialog

}
}

#endif // _LR_MAIN_DIALOG_H_
