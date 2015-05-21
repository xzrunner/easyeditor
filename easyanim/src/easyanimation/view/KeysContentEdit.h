#ifndef _EASYANIM_KEYS_CONTENT_EDIT_H_
#define _EASYANIM_KEYS_CONTENT_EDIT_H_

#include <vector>

namespace eanim
{

class Controller;
class KeyFrame;

class KeysContentEdit
{
public:
	KeysContentEdit(Controller* ctrl);

	void OnMouseLeftDown(int row, int col);
	void OnMouseLeftUp(int row, int col);
	void OnMouseDragging(int row, int col);

	void CopySelection();
	void PasteSelection();
	void DeleteSelection();

private:
	Controller* m_ctrl;

	int m_last_row, m_last_col;

}; // KeysContentEdit

}

#endif // _EASYANIM_KEYS_CONTENT_EDIT_H_