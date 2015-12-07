#ifndef _EASYANIM_KEYS_CONTENT_EDIT_H_
#define _EASYANIM_KEYS_CONTENT_EDIT_H_

#include <vector>

namespace eanim
{

class KeysContentEdit
{
public:
	KeysContentEdit();

	void OnMouseLeftDown(int row, int col);
	void OnMouseDragging(int row, int col);

	void CopySelection();
	void PasteSelection();
	void DeleteSelection();

private:
	void UpdateRegion(int layer, int frame);

	bool IsSelectionValid() const;

private:
	int m_row, m_col;
	int m_col_min, m_col_max;

}; // KeysContentEdit

}

#endif // _EASYANIM_KEYS_CONTENT_EDIT_H_