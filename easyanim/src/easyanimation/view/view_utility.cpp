#include "view_utility.h"
#include "ViewMgr.h"
#include "KeysPanel.h"

#include "dataset/KeyFrame.h"
#include "dataset/DataMgr.h"

#include <drag2d.h>

namespace eanim
{

void reload_viewlist(const KeyFrame* frame)
{
	if (!frame) {
		return;
	}

	d2d::ViewlistPanel* vl = ViewMgr::Instance()->viewlist;

	vl->Clear();
	const std::vector<d2d::ISprite*>& sprites = frame->GetAllSprites();
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		vl->Insert(sprites[i]);
	}
}

int get_curr_layer_index()
{
	int row, col;
	ViewMgr::Instance()->keys->GetSelectPos(row, col);
	return DataMgr::Instance()->GetLayers().Size() - 1 - row;
}

int get_curr_frame_index()
{
	int row, col;
	ViewMgr::Instance()->keys->GetSelectPos(row, col);
	return col;
}

void get_curr_index(int& layer_idx, int& frame_idx)
{
	int row, col;
	ViewMgr::Instance()->keys->GetSelectPos(row, col);
	layer_idx = DataMgr::Instance()->GetLayers().Size() - 1 - row;
	frame_idx = col;
}

}