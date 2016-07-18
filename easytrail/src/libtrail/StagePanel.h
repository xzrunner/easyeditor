#ifndef _EASYTRAIL_STAGE_PANEL_H_
#define _EASYTRAIL_STAGE_PANEL_H_

#include <ee/EditPanel.h>
#include <ee/SpritesPanelImpl.h>

#include <SM_Matrix.h>

namespace ee { class LibraryPanel; class PropertySettingPanel; }

namespace etrail
{

class StageCanvas;
class MotionTrail;

class StagePanel : public ee::EditPanel, public ee::SpritesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		ee::LibraryPanel* library, ee::PropertySettingPanel* property);
	virtual ~StagePanel();

	virtual bool Update(float dt);

	void SetTrailMat(const sm::vec2& pos);
	const sm::mat4& SetTrailMat() const { return m_mat; }

public:
	MotionTrail* m_trail;

private:
	sm::mat4 m_mat;

}; // StagePanel

}

#endif // _EASYTRAIL_STAGE_PANEL_H_