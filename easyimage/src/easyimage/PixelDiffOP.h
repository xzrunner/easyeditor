#ifndef _EASYIMAGE_PIXEL_DIFF_OP_H_
#define _EASYIMAGE_PIXEL_DIFF_OP_H_



namespace eimage
{

class StagePanel;

class PixelDiffOP : public ee::ZoomViewOP
{
public:
	PixelDiffOP(StagePanel* stage);

	virtual bool OnMouseLeftDown(int x, int y);

	virtual bool OnDraw() const;
	virtual bool Clear();

	virtual bool OnActive();

private:
	StagePanel* m_stage;

//	ee::ImageSprite *m_left, *m_right;

}; // PixelDiffOP 

}

#endif // _EASYIMAGE_PIXEL_DIFF_OP_H_