#ifndef _ESHADER_STAGE_CANVAS2D_H_
#define _ESHADER_STAGE_CANVAS2D_H_



namespace eshader
{

class StagePanel2D;

class StageCanvas2D : public ee::OrthoCanvas
{
public:
	StageCanvas2D(StagePanel2D* stage);
	virtual ~StageCanvas2D();

	void OnMousePressed(const ee::Vector& pos);

protected:
	virtual void OnDrawSprites() const;

private:
	void DrawBackground() const;
	void DrawSprites() const;

	void OnTimer(wxTimerEvent& event);

private:
	enum
	{
		TIMER_ID = 1000
	};

private:
	wxTimer m_timer;

	StagePanel2D* m_stage;

	clock_t m_start_time;

	DECLARE_EVENT_TABLE()

}; // StageCanvas2D

}

#endif // _ESHADER_STAGE_CANVAS2D_H_