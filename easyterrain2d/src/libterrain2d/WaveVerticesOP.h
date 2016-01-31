#ifndef _EASYTERRAIN2D_WAVE_VERTICES_OP_H_
#define _EASYTERRAIN2D_WAVE_VERTICES_OP_H_



namespace eterrain2d
{

class StagePanel;

class WaveVerticesOP : public ee::ZoomViewOP
{
public:
	WaveVerticesOP(StagePanel* stage);

	virtual bool OnDraw() const;

private:
	StagePanel* m_stage;

	bool m_draw_tris;

	friend class WaveVerticesCMPT;

}; // WaveVerticesOP

}

#endif // _EASYTERRAIN2D_WAVE_VERTICES_OP_H_