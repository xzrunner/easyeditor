#ifndef _EASYCOMPLEX_PREVIEW_EDITOP_H_
#define _EASYCOMPLEX_PREVIEW_EDITOP_H_



namespace ecomplex
{

class PreviewEditOP : public ee::ZoomViewOP
{
public:
	PreviewEditOP(wxWindow* wnd, ee::EditPanelImpl* stage, 
		const std::vector<const ee::Sprite*>& sprites);

	virtual bool OnMouseLeftDown(int x, int y);

private:
	const std::vector<const ee::Sprite*>& m_sprites;

}; // PreviewEditOP

}

#endif // _EASYCOMPLEX_PREVIEW_EDITOP_H_