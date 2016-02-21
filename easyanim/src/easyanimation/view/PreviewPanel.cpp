#include "PreviewPanel.h"
#include "PreviewUtility.h"

#include <ee/SceneNode.h>
#include <ee/Sprite.h>

namespace eanim
{

PreviewPanel::PreviewPanel(wxWindow* parent, wxTopLevelWindow* frame,
						   ee::PlayControl& ctrl)
	: ee::EditPanel(parent, frame)
	, m_ctrl(ctrl)
{
}

bool PreviewPanel::Update(int version)
{
	ee::SceneNodeMgr::Instance()->Update(1 / 30.0f);

	std::vector<ee::Sprite*> sprites;
	PreviewUtility::GetCurrSprites(m_ctrl, sprites);

	for (int i = 0, n = sprites.size(); i < n; ++i) {
		sprites[i]->Update(version);
	}

	for (size_t i = 0, n = sprites.size(); i < n; ++i) {
		sprites[i]->Release();
	}

	return true;
}

}