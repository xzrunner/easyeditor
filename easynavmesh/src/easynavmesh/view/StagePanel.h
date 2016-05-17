#ifndef _EASYNAVMESH_STAGE_PANEL_H_
#define _EASYNAVMESH_STAGE_PANEL_H_

#include <ee/EditPanel.h>
#include <ee/SpritesPanelImpl.h>
#include <ee/ShapesPanelImpl.h>

namespace ee { class PropertySettingPanel; }

namespace enav
{

class Mesh;

class StagePanel : public ee::EditPanel, public ee::SpritesPanelImpl, public ee::ShapesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		ee::LibraryPanel* library, ee::PropertySettingPanel* property);
	virtual ~StagePanel();

	const sm::rect& GetRegion() const { return m_region; }

	const Mesh* GetMesh() const { return m_mesh; }

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	sm::rect m_region;

	Mesh* m_mesh;

}; // StagePanel

}

#endif // _EASYNAVMESH_STAGE_PANEL_H_