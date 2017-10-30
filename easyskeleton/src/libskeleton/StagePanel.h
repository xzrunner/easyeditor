#ifndef _LIBSKELETON_STAGE_PANEL_H_
#define _LIBSKELETON_STAGE_PANEL_H_

#include <ee/EditPanel.h>

class wxGLContext;

namespace s2 { class Skeleton; }

namespace libskeleton
{

class StagePanel : public ee::EditPanel
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		wxGLContext* glctx, s2::Skeleton& sk);

	//
	// ee::EditPanel interface
	//
	virtual bool UpdateStage();

	auto& GetSkeleton() const { return m_sk; }

private:
	s2::Skeleton& m_sk;

}; // StagePanel

}

#endif // _LIBSKELETON_STAGE_PANEL_H_