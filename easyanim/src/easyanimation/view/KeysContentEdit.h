#ifndef _EASYANIM_KEYS_CONTENT_EDIT_H_
#define _EASYANIM_KEYS_CONTENT_EDIT_H_

#include <vector>

namespace eanim
{

class LayersMgr;
class KeysPanel;

class KeysContentEdit
{
public:
	KeysContentEdit(const LayersMgr& layers, KeysPanel& keys_panel);

	void OnMouseLeftDown(int row, int col);
	void OnMouseLeftUp(int row, int col);
	void OnMouseDragging(int row, int col);

	void CopySelection();
	void PasteSelection();
	void DeleteSelection();

private:
	const LayersMgr& m_layers;

	KeysPanel& m_keys_panel;

	int m_last_row, m_last_col;

}; // KeysContentEdit

}

#endif // _EASYANIM_KEYS_CONTENT_EDIT_H_