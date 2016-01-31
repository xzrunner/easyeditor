#ifndef _LR_MAIN_DIALOG_H_
#define _LR_MAIN_DIALOG_H_



namespace lr
{
namespace preview
{

class MainDialog : public wxDialog
{
public:
	MainDialog(wxWindow* parent, int width, int height, 
		const std::vector<const ee::Sprite*>& sprites);

private:
	void InitLayout();

private:
	std::vector<const ee::Sprite*> m_sprites;

	ee::PlayControl m_control;

}; // MainDialog

}
}

#endif // _LR_MAIN_DIALOG_H_
