#include "PreviewPanel.h"
#include "PreviewUtility.h"

namespace eanim
{

PreviewPanel::PreviewPanel(wxWindow* parent, wxTopLevelWindow* frame,
						   d2d::PlayControl& ctrl)
	: d2d::EditPanel(parent, frame)
	, m_ctrl(ctrl)
{
}

bool PreviewPanel::Update(int version)
{
	d2d::SceneNodeMgr::Instance()->Update(1 / 30.0f);

	std::vector<d2d::Sprite*> sprites;
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