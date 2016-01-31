#include "ViewMgr.h"

#include "dataset/DataMgr.h"
#include "dataset/KeyFrame.h"
#include "dataset/Layer.h"
#include "message/messages.h"
#include "view/KeyFramePropertySetting.h"

namespace eanim
{

ViewMgr* ViewMgr::m_instance = NULL;

ViewMgr* ViewMgr::Instance()
{
	if (!m_instance) {
		m_instance = new ViewMgr;
	}
	return m_instance;
}

ViewMgr::ViewMgr()
{
	library = NULL;
	property = NULL;
	img_page = NULL;

	toolbar = NULL;
	stage = NULL;
	layers = NULL;
	keys = NULL;

	viewlist = NULL;

	m_layer = m_frame = -1;

	RegistSubject(SetSelectedSJ::Instance());

}

void ViewMgr::OnNotify(int sj_id, void* ud)
{
	switch (sj_id)
	{
	case MSG_SET_CURR_FRAME:
		{
			SetSelectedSJ::Position* cf = (SetSelectedSJ::Position*)ud;
			if (cf->layer == -1 && cf->frame == -1) {
				m_layer = m_frame = -1;
				viewlist->Clear();
			} else {
				if (cf->layer != -1) {
					m_layer = cf->layer;
				}
				if (cf->frame != -1) {
					m_frame = cf->frame;
				}
				Layer* layer = DataMgr::Instance()->GetLayers().GetLayer(m_layer);
				KeyFrame* frame = layer->GetCurrKeyFrame(m_frame + 1);
				UpdateProperty(frame);
				UpdateViewlist(frame);
			}
		}
		break;
	}
}

void ViewMgr::UpdateProperty(KeyFrame* frame)
{
	KeyFramePropertySetting* setting = NULL;
	if (frame) {
		setting = new KeyFramePropertySetting(frame);
	}
	property->SetPropertySetting(setting);
}

void ViewMgr::UpdateViewlist(KeyFrame* frame)
{
	viewlist->Clear();
	if (!frame) {
		return;
	}

	const std::vector<ee::Sprite*>& sprites = frame->GetAllSprites();
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		viewlist->Insert(sprites[i]);
	}
}

}