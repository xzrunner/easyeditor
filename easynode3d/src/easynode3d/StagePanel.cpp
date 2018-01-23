#include "StagePanel.h"
#include "StageCanvas.h"
#include "StageDropTarget.h"

#include <ee/Sprite.h>
#include <ee/sprite_msg.h>
#include <ee/panel_msg.h>

namespace enode3d
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   ee::LibraryPanel* library)
	: ee::EditPanel(parent, frame)
{
	SetDropTarget(new StageDropTarget(this, this, library));
}

sm::vec3 StagePanel::TransPosScrToProj(int x, int y) const
{
	auto canvas = std::dynamic_pointer_cast<const StageCanvas>(GetCanvas());
	if (!canvas) {
		return sm::vec3();
	}

	auto dir = canvas->TransPos3ScreenToDir(sm::vec2(x, y));
	auto& cam = canvas->GetCamera();
	sm::vec3 ret = dir.Normalized() * cam.GetDistance();
	ret.y = 0;
	return ret;
}

void StagePanel::InsertNode(const n3::NodePtr& node)
{
	m_nodes.push_back(node);
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void StagePanel::DeleteNode(const n3::NodePtr& node)
{
	bool dirty = false;
	for (auto itr = m_nodes.begin(); itr != m_nodes.end(); ++itr) {
		if (*itr == node) {
			dirty = true;
			m_nodes.erase(itr);
			break;
		}
	}

	if (dirty) {
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

}