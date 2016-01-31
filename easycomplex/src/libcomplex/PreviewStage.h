#ifndef _EASYCOMPLEX_PREVIEW_STAGE_H_
#define _EASYCOMPLEX_PREVIEW_STAGE_H_



namespace ecomplex
{

class PreviewStage : public ee::EditPanel
{
public:
	PreviewStage(wxWindow* parent, wxTopLevelWindow* frame,
		ee::PlayControl& control);
	
	virtual bool Update(int version);
	
private:
	ee::PlayControl& m_control;

}; // PreviewStage

}

#endif // _EASYCOMPLEX_PREVIEW_STAGE_H_