#ifndef _EASYANIM_KEYS_PANEL_H_
#define _EASYANIM_KEYS_PANEL_H_



namespace eanim
{

class Layer;

class KeysPanel : public wxScrolledWindow, public ee::Observer
{
public:
	KeysPanel(wxWindow* parent);

	void GetSelectPos(int& row, int& col) const;

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	void InitLayout();

private:
	int m_selected_row, m_selected_col;

	Layer* m_layer;

}; // KeysPanel

}

#endif // _EASYANIM_KEYS_PANEL_H_